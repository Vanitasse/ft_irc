/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mablatie <mablatie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/22 16:23:16 by vanitas           #+#    #+#             */
/*   Updated: 2024/05/24 16:27:08 by mablatie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Client.hpp"

Client::Client() {}

Client::~Client() {}

Client::Client(const Client& cpy)
{
	*this = cpy;
}

Client& Client::operator=(const Client& rhs)
{
	if (this != &rhs)
	{
		this->fd = rhs.getFd();
		this->ip_addr = rhs.getIp();
	}
	return *this;
}

int Client::getFd() const
{
	return this->fd;
}

void Client::setFd(int fd_input)
{
	this->fd = fd_input;
}

const std::string& Client::getIp() const
{
	return this->ip_addr;
}

void Client::setIp(int ip_input)
{
	this->ip_addr = ip_input;
}

int Client::getNb() const
{
	return this->nb;
}

void Client::setNb(int nb_input)
{
	this->nb = nb_input;
}

const std::string& Client::getUsername() const
{
	return this->username;
}

void Client::setUsername(std::string user_input)
{
	this->username = user_input;
}