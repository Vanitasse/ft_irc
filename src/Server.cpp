/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bvaujour <bvaujour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/27 14:42:57 by bvaujour          #+#    #+#             */
/*   Updated: 2024/05/28 10:32:05 by bvaujour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Server.hpp"

Server::Server() : serv_sock_fd(-1)
{

}

Server::~Server()
{
	closeFds();
}

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
