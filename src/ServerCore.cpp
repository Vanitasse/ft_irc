/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerCore.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bvaujour <bvaujour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/22 14:45:42 by bvaujour          #+#    #+#             */
/*   Updated: 2024/05/27 16:32:43 by bvaujour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Server.hpp"

bool Server::signal = false;

void Server::serverInit()
{
	struct pollfd		new_poll;
	struct sockaddr_in	sock_addr; //structure pour l'adresse internet
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
				if (i == 0)
					connectClient();
				else
					readData(clients[i - 1]);
			}
		}
	}
}

void Server::connectClient()
{
	Client client;
	struct sockaddr_in sock_addr;
	socklen_t len = sizeof(sock_addr);
	struct pollfd		new_poll;

	client.setState(LOGIN);
	client.setChannel("general");
	client.setFd(accept(this->serv_sock_fd, (struct sockaddr *)&sock_addr, &len));
	if (client.getFd() == -1)
		throw std::runtime_error("Client socket creation failed");
	if (fcntl(client.getFd(), F_SETFL, O_NONBLOCK) == -1) //met l'option O_NONBLOCK pour faire une socket non bloquante
		throw(std::runtime_error("set option (O_NONBLOCK) failed"));
	new_poll.fd = client.getFd();
	new_poll.events = POLLIN;
	new_poll.revents = 0;
	poll_fds.push_back(new_poll);
	clients.push_back(client);
	send(client.getFd(), "Enter your username : ", 23, 0);
}

// void	Server::Emoticon()
// {
	
// }
void	Server::readData(Client& client)
{
	std::string	input;
	char buffer[1024];
	memset(buffer, 0,1024);
	ssize_t bytes;

	bytes = recv(client.getFd(), buffer, sizeof(buffer), 0); //MSG_WAITALL MSG_DONTWAIT MSG_PEEK MSG_TRUNC
	if (bytes <= 0)
		clearClient(client.getFd());
	else
	{
		buffer[bytes] = '\0';
		input = buffer;
		switch	(client.getState())
		{
			case	LOGIN:
				if (input.length() > 0)
					input[input.length() - 1] = '\0';
				client.setUsername(input);
				client.setState(PASSWORD);
				send(client.getFd(), "Enter Server password : ", 25, 0);
				return ;
			case	PASSWORD:
				if (input == password + '\n')
				{
					send(client.getFd(), "Connected\n", 10, 0);
					client.setState(CONNECTED);
				}
				else
					send(client.getFd(), "Enter Server password : ", 25, 0);
				return ;
			case	CONNECTED:
				for (std::vector<Client>::iterator it = clients.begin(); it < clients.end(); it++)
				{
					if (it->getFd() != client.getFd())
					{
						send(it->getFd(), client.getUsername().c_str(), client.getUsername().size(), 0);
						send(it->getFd(), ": ", 2, 0);
						send(it->getFd(), buffer, sizeof(buffer), 0);
					}
				}
				return ;
		}
	}
}
