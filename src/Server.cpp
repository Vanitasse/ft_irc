/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vanitas <vanitas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/22 14:45:42 by bvaujour          #+#    #+#             */
/*   Updated: 2024/05/22 17:44:29 by vanitas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Server.hpp"

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
	this->signal = false;
	struct sockaddr_in sock_addr; //structure pour l'adresse internet
	struct pollfd poll; // structure pour les polling request = les events donc message/reception
	sock_addr.sin_family = AF_INET; //on set le type en IPV4
	sock_addr.sin_port = htons(this->port); // converti le port(int) en big endian (pour le network byte order)
	sock_addr.sin_addr.s_addr = INADDR_ANY; // IMADDR_ANY = n'importe quel adresse donc full local

	this->serv_sock_fd = socket(AF_INET, SOCK_STREAM, 0); //socket creation
	if (serv_sock_fd == -1)
		throw(std::runtime_error("socket creation failed"));

	int uh = 1; // necessaire car parametre opt_value = const void *
	if (setsockopt(serv_sock_fd, SOL_SOCKET, SO_REUSEADDR, &uh, sizeof(uh)) == - 1) //met l'option SO_REUSEADDR sur la socket = permet de reutiliser l'addresse
		throw(std::runtime_error("set option (SO_REUSEADDR) failed"));
	if (fcntl(serv_sock_fd, F_SETFL, O_NONBLOCK) == -1) //met l'option O_NONBLOCK pour faire une socket non bloquante
		throw(std::runtime_error("set option (O_NONBLOCK) failed"));
	if (bind(serv_sock_fd, (struct sockaddr*) &sock_addr, sizeof(sock_addr)) == -1) //lie la socket a l'adresse reutilisable (obliger de cast pour avoir toute les infos)
		throw(std::runtime_error("socket binding failed"));
	if (listen(serv_sock_fd, SOMAXCONN) == -1) //la socket devient passive = c'est une socket serveur qui attend des connections (SOMAXCONN = max_connections autorise)
		throw(std::runtime_error("listen() failed"));

	poll.fd = serv_sock_fd; //enregistre la socket dans le poll
	poll.events = POLLIN; //le poll doit lire des infos;
	poll.revents = 0; //0 events actuellement donc par defaut;
	

}

void Server::serverExec()
{
	while (this->signal == false)
	{
		
	}
}


void Server::closeFds()
{
	//client
	;
	//server
	if (this->serv_sock_fd != -1)
		close(serv_sock_fd);
}


void Server::signalHandler(int signum)
{
	(void)signum;
	Server::signal = true;
}
