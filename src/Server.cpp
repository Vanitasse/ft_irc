/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mablatie <mablatie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/22 14:45:42 by bvaujour          #+#    #+#             */
/*   Updated: 2024/05/24 17:34:13 by mablatie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Server.hpp"

bool Server::signal = false;

Server::Server() : serv_sock_fd(-1) {}

Server::~Server() {}

Server::Server(const Server& toCpy)
{
	(void)toCpy;
}

Server&	Server::operator=(const Server& toCpy)
{
	(void)toCpy;
	return (*this);
}

Server::Server(const std::string& port, const std::string& password)
{
	this->port = atoi(port.c_str());
	this->password = password;
	
}

void Server::closeFds()
{
	for (size_t i = 0; i < clients.size(); i++)
	{
		std::cout << "Client " << clients[i].getFd() << " disconnected." << std::endl;
		close(clients[i].getFd());	
	}
	if (this->serv_sock_fd != -1)
		close(serv_sock_fd);
}


void Server::signalHandler(int signum) //static
{
	(void)signum;
	std::cout << "SIGNAL RECEIVED" << std::endl;
	Server::signal = true;
}

void Server::serverInit()
{
	struct sockaddr_in sock_addr; //structure pour l'adresse internet
	struct pollfd new_poll; // structure pour les polling request = les events donc message/reception
	int uh = 1; // necessaire car parametre opt_value de setsockopt() = const void *
	sock_addr.sin_family = AF_INET; //on set le type en IPV4
	sock_addr.sin_port = htons(this->port); // converti le port(int) en big endian (pour le network byte order)
	sock_addr.sin_addr.s_addr = INADDR_ANY; // IMADDR_ANY = n'importe quel adresse donc full local

	this->serv_sock_fd = socket(AF_INET, SOCK_STREAM, 0); //socket creation
	if (serv_sock_fd == -1)
		throw(std::runtime_error("socket creation failed"));

	if (setsockopt(serv_sock_fd, SOL_SOCKET, SO_REUSEADDR, &uh, sizeof(uh)) == - 1) //met l'option SO_REUSEADDR sur la socket = permet de reutiliser l'addresse
		throw(std::runtime_error("set option (SO_REUSEADDR) failed"));
	if (fcntl(serv_sock_fd, F_SETFL, O_NONBLOCK) == -1) //met l'option O_NONBLOCK pour faire une socket non bloquante
		throw(std::runtime_error("set option (O_NONBLOCK) failed"));
	if (bind(serv_sock_fd, (struct sockaddr*) &sock_addr, sizeof(sock_addr)) == -1) //lie la socket a l'adresse reutilisable (obliger de cast pour avoir toute les infos)
		throw(std::runtime_error("socket binding failed"));
	if (listen(serv_sock_fd, SOMAXCONN) == -1) //la socket devient passive = c'est une socket serveur qui attend des connections (SOMAXCONN = max_connections autorise)
		throw(std::runtime_error("listen() failed"));

	new_poll.fd = serv_sock_fd; //enregistre la socket dans le poll
	new_poll.events = POLLIN; //le poll doit lire des infos;
	new_poll.revents = 0; //0 events actuellement donc par defaut;
	poll_fds.push_back(new_poll);
	serverExec();
}


void Server::serverExec()
{
	while (this->signal == false)
	{
		if (poll(&poll_fds[0], poll_fds.size(), -1) == -1 && this->signal == false) //-1 = bloque ici attend un event, 0 = bloque pas tourne dans le while
			throw(std::runtime_error("poll() function failed"));
		for (size_t i = 0; i < poll_fds.size(); i++)
		{
			if (poll_fds[i].revents && POLLIN)
			{
				if (poll_fds[i].fd == this->serv_sock_fd)
					connectClient();
				else
					readData(poll_fds[i].fd);	
			}
		}
	}
	closeFds();
}

void Server::PromptName(Client& cli)
{
	std::string input;
	char buffer[10];
	ssize_t bytes;
	
	bytes = 0;
	send(cli.getFd(), "Enter your username : ", 23, 0);
	while (bytes == 0 && signal == false)
		bytes = recv(cli.getFd(), buffer, sizeof(buffer), 0);
	input = buffer;
	if (input.size() > 1)
		input[input.size() - 1] = '\0';
	cli.setUsername(input);
	std::cout << "Username " << cli.getUsername() << " connected" << std::endl;
}

void Server::connectClient()
{
	Client cli;
	struct pollfd new_poll;
	struct sockaddr_in sock_addr;
	socklen_t len = sizeof(sock_addr);

	std::cout << "CONNECT" << std::endl;
	cli.setFd(accept(this->serv_sock_fd, (struct sockaddr *)&sock_addr, &len));
	if (cli.getFd() == -1)
		throw std::runtime_error("Client socket creation failed");
	if (fcntl(cli.getFd(), F_SETFL, O_NONBLOCK) == -1) //met l'option O_NONBLOCK pour faire une socket non bloquante
		throw(std::runtime_error("set option (O_NONBLOCK) failed"));
	PromptName(cli);
		
	new_poll.fd = cli.getFd();
	new_poll.events = POLLIN;
	new_poll.revents = 0;
	clients.push_back(cli);
	clients.front().setNb(clients.size());
	poll_fds.push_back(new_poll);
}

void Server::readData(int fd)
{
	char buffer[20];
	ssize_t bytes;
	
	bytes = recv(fd, buffer, sizeof(buffer), 0); //MSG_WAITALL MSG_DONTWAIT MSG_PEEK MSG_TRUNC
	if (bytes <= 0)
		clearClient(fd);
	else
	{
		buffer[bytes] = '\0';
		std::cout << "Buffer = " << buffer;
		//Interpretation de la data ici
	}
}

void Server::clearClient(int fd)
{
	for (std::vector<pollfd>::iterator it = poll_fds.begin(); it < poll_fds.end(); it++)
	{	
		if (fd == it->fd)
		{
			poll_fds.erase(it);
			break;
		}
	}
	for (std::vector<Client>::iterator it = clients.begin(); it < clients.end(); it++)
	{
		if (fd == it->getFd())
		{
			std::cout << "Client " << it->getUsername() << " disconnected" << std::endl;
			clients.erase(it);
			break;
		}
	}
	close(fd);	
}
