/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bvaujour <bvaujour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/30 23:14:13 by bvaujour          #+#    #+#             */
/*   Updated: 2024/05/31 17:26:02 by bvaujour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Server.hpp"

bool Server::_signal = false;

Server::Server()
{
	
}

Server::~Server()
{
	std::vector<Client*>::iterator			it;
	std::cout << _Clients.size() << std::endl;
	it = _Clients.begin();
	while (it != _Clients.end())
	{
		close ((*it)->getFd());
		delete *it;
		it++;
	}
	close (_pfds[0].fd);
}

Server::Server(const Server& toCpy)
{
	*this = toCpy;
}

Server&	Server::operator=(const Server& toCpy)
{
	if (this != &toCpy)
	{
		
	}
	return (*this);
}

Server::Server(const int& port, const std::string password_input) : _password(password_input)
{
	struct pollfd		new_poll= {};
	struct sockaddr_in	sock_addr; //structure pour l'adresse internet
	int uh = 1; // necessaire car parametre opt_value de setsockopt() = const void *

	sock_addr.sin_family = AF_INET; //on set le type en IPV4
	sock_addr.sin_port = htons(port); // converti le port(int) en big endian (pour le network byte order)
	sock_addr.sin_addr.s_addr = INADDR_ANY; // IMADDR_ANY = n'importe quel adresse donc full local

	new_poll.fd = socket(AF_INET, SOCK_STREAM, 0); //socket creation
	if (new_poll.fd == -1)
		throw(std::runtime_error("socket creation failed"));

	if (setsockopt(new_poll.fd, SOL_SOCKET, SO_REUSEADDR, &uh, sizeof(uh)) == - 1) //met l'option SO_REUSEADDR sur la socket = permet de reutiliser l'addresse
		throw(std::runtime_error("set option (SO_REUSEADDR) failed"));
	if (fcntl(new_poll.fd, F_SETFL, O_NONBLOCK) == -1) //met l'option O_NONBLOCK pour faire une socket non bloquante
		throw(std::runtime_error("set option (O_NONBLOCK) failed"));
	if (bind(new_poll.fd, (struct sockaddr*) &sock_addr, sizeof(sock_addr)) == -1) //lie la socket a l'adresse reutilisable (obliger de cast pour avoir toute les infos)
		throw(std::runtime_error("socket binding failed"));
	if (listen(new_poll.fd, SOMAXCONN) == -1) //la socket devient passive = c'est une socket serveur qui attend des connections (SOMAXCONN = max_connections autorise)
		throw(std::runtime_error("listen() failed"));

	new_poll.events = POLLIN; //le poll doit lire des infos;
	new_poll.revents = 0; //0 events actuellement donc par defaut;
	_pfds.push_back(new_poll); 
	serverExec();
}

void Server::serverExec()
{
	while (this->_signal == false)
	{
		if (poll(&_pfds[0], _pfds.size(), -1) == -1 && this->_signal == false) //-1 = bloque ici attend un event, 0 = bloque pas tourne dans le while
			throw(std::runtime_error("poll() function failed"));
		for (size_t i = 0; i < _pfds.size(); i++)
		{
			if (_pfds[i].revents && POLLIN)
			{
				if (i == 0)
					connectClient();
				else
					readData(*_Clients[i - 1]);
			}
		}
	}
}

void	Server::addIrssiClient(int fd)
{
	Client	*client = new IrssiClient();

	std::cout << "irssi Client connected" << std::endl;
	const char *welcome_msg = ":localhost 001 user :Welcome to the IRC server\r\n";
	send(fd, welcome_msg, strlen(welcome_msg), 0);
	client->setFd(fd);
	_Clients.push_back(client);
}

void	Server::addNcClient(int fd)
{
	Client	*client = new NcClient();

	std::cout << "nc Client connected" << std::endl;
	client->setFd(fd);
	_Clients.push_back(client);
}
void	Server::connectClient()
{
	struct sockaddr_in sock_addr;
	socklen_t len = sizeof(sock_addr);
	struct pollfd		new_poll = {};

	std::cout << "connectClient" << std::endl;
	new_poll.fd = accept(_pfds[0].fd, (struct sockaddr *)&sock_addr, &len);
	if (new_poll.fd == -1)
		throw std::runtime_error("Client socket creation failed");
	if (fcntl(new_poll.fd, F_SETFL, O_NONBLOCK) == -1) //met l'option O_NONBLOCK pour faire une socket non bloquante
		throw(std::runtime_error("set option (O_NONBLOCK) failed"));
	new_poll.events = POLLIN;
	new_poll.revents = 0;
	_pfds.push_back(new_poll);
	if (ServerRecv(new_poll.fd))
		addIrssiClient(new_poll.fd);
	else
		addNcClient(new_poll.fd);
}

void	Server::readData(Client& client)
{
	std::cout << "readData" << std::endl;
	if (!ServerRecv(client.getFd()))
	{
		clearClient(client);
		return ;
	}
	ServerSend(client);
}
