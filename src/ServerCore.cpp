/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerCore.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mablatie <mablatie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/22 14:45:42 by bvaujour          #+#    #+#             */
/*   Updated: 2024/05/28 17:01:05 by mablatie         ###   ########.fr       */
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
	createChannel("#general", NULL);
	serverExec();
}


void Server::connectClient()
{
	Client client;
	struct sockaddr_in sock_addr;
	socklen_t len = sizeof(sock_addr);
	struct pollfd		new_poll;

	client.setState(LOGIN);
	client.setChannel("#general");
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
	client.printPrompt("Enter your username : ");
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


void	Server::readData(Client& client)
{
	char buffer[1024];
	memset(buffer, 0,1024);
	ssize_t bytes;
	
	bytes = recv(client.getFd(), buffer, sizeof(buffer), 0); //MSG_WAITALL MSG_DONTWAIT MSG_PEEK MSG_TRUNC
	if (bytes <= 0)
		clearClient(client.getFd());
	else
	{
		buffer[bytes] = '\0';
		switch	(client.getState())
		{	
			case	LOGIN:
				return (client.loginRecv(buffer));
			case	PASSWORD:
				return (client.passwordRecv(buffer, password));
			case	CONNECTED:
			{
				if(handleCommands(buffer, client))
					return (client.connectedRecv(buffer, this->clients));
			}
		}
	}
}



void Server::createChannel(std::string input, Client *client)
{
	Channel new_channel;
	new_channel.setName(input);
	if (client != NULL)
	{
		client->setChannel(input);
		new_channel.setOperateur(client);
		std::cout << "Operator = " << new_channel.getOperateur()->getUsername() << std::endl;
	}
	channels.push_back(new_channel);
}

// void Server::pushToChannel(Client& client)
// {
// 	for (size_t i = 0; i < channels.size(); i++)
// 	{
// 		if (client.getChannel() == channels[i].getName())
// 		{
// 			channels[i].getChanClients().push_back(&client);
// 		}
// 	}
	
// }

int	Server::handleCommands(std::string buffer, Client& client)
{
	if (buffer.find("/JOIN", 0) != buffer.npos)
	{
		this->JOIN(buffer, client);
		return 0;
	}
	else if (buffer.find("/KICK", 0) != buffer.npos)
	{
		this->KICK(buffer, client);
		return 0;
	}
	else if (buffer.find("/INVITE", 0) != buffer.npos)
		return 0;
	else if (buffer.find("/TOPIC", 0) != buffer.npos)
		return 0;
	else if (buffer.find("/MODE", 0) != buffer.npos)
		return 0;
	return 1;
}

void	Server::JOIN(std::string buffer, Client& client)
{
	size_t pos;

	if (buffer.length() > 0)
		buffer[buffer.length() - 1] = '\0';
	pos = buffer.find('#', 6);
	if (pos != buffer.npos)
	{
		for (size_t i = 0; i < clients.size(); i++)
		{
			if (clients[i].getChannel() == buffer.substr(pos, buffer.size() - 1))
			{
				client.setChannel(clients[i].getChannel());
				client.printPrompt(std::string(GREEN) + client.getChannel() + ": " + RESET);
				return ;
			}
		}
		createChannel(buffer.substr(pos, buffer.size()), &client);
		client.printPrompt(std::string(GREEN) + client.getChannel() + ": " + RESET);
	}
}

void	Server::INVITE()
{

}

void	Server::KICK(std::string buffer, Client& client)
{
	for (size_t i = 1; i < channels.size(); i++)
	{
		if (client.getChannel() == channels[i].getName())
		{
			std::cout << "1 OUI" << std::endl;
			if (channels[i].getOperateur()->getUsername() == client.getUsername())
			{
				
				
			}
			
		}
	}
	(void)buffer;
}

void	Server::MODE()
{
	
}
