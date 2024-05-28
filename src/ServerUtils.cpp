/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerUtils.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bvaujour <bvaujour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/27 14:28:29 by bvaujour          #+#    #+#             */
/*   Updated: 2024/05/28 10:30:07 by bvaujour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Server.hpp"

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

void Server::clearClient(int fd)
{
	std::cout << "Entering clearClient" << std::endl;
	for (std::vector<struct pollfd>::iterator it = poll_fds.begin(); it < poll_fds.end(); it++)
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
