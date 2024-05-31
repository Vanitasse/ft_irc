/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerUtils.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bvaujour <bvaujour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/27 14:28:29 by bvaujour          #+#    #+#             */
/*   Updated: 2024/05/31 17:55:28 by bvaujour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Server.hpp"

int	Server::ServerRecv(int fd)
{
	char buffer[1024] = {0};
	ssize_t bytes;

	bytes = recv(fd, buffer, sizeof(buffer), 0); //MSG_WAITALL MSG_DONTWAIT MSG_PEEK MSG_TRUNC
	if (bytes <= 0)
	{
		std::cout << "Sortie" <<std::endl;
		return (0);
	}
	buffer[bytes] = '\0';
	_strBuf = buffer;
	int	i(0);

	std::cout << CYAN << "[Server receive]:";
	while (_strBuf[i])
	{
		if (_strBuf[i] == '\r')
			std::cout << "\\r";
		else if (_strBuf[i] == '\n')
			std::cout << "\\n";
		else
			std::cout << _strBuf[i];
		i++;
	}
	std::cout << RESET << std::endl;
	return (1);
}

void	Server::ServerSend(Client& Sender)
{
	std::vector<Client*>::iterator	it;
	it = _Clients.begin();
	while (it != _Clients.end())
	{
		if (*it != &Sender)
			send((*it)->getFd(), _strBuf.c_str(), _strBuf.size(), 0);
		it++;
	}
}

void	Server::clearClient(Client& client)
{
	std::vector<Client*>::iterator			it;
	std::vector<struct pollfd>::iterator	it2;

	std::cout << "Entering ClearClient" << std::endl;
	it2 = _pfds.begin();
	while (it2 != _pfds.end() && it2->fd != client.getFd())
		it2++;
	_pfds.erase(it2);
	it = _Clients.begin();
	while (it != _Clients.end() && (*it)->getFd() != client.getFd())
		it++;
	close((*it)->getFd());
	delete *it;
	_Clients.erase(it);
}

void Server::signalHandler(int signum) //static
{
	(void)signum;
	std::cout << "SIGNAL RECEIVED" << std::endl;
	Server::_signal = true;
}
