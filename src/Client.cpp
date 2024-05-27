/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bvaujour <bvaujour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/22 16:23:16 by vanitas           #+#    #+#             */
/*   Updated: 2024/05/27 15:35:52 by bvaujour         ###   ########.fr       */
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
		this->username = rhs.getUsername();
		this->channel = rhs.getChannel();
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

const std::string& Client::getUsername() const
{
	return this->username;
}

void Client::setUsername(std::string user_input)
{
	this->username = user_input;
}

const std::string& Client::getChannel() const
{
	return this->channel;
}

void Client::setChannel(std::string channel_input)
{
	this->username = channel_input;
}

const State& Client::getState() const
{
	return this->state;
}

void Client::setState(const State& state_input)
{
	this->state = state_input;
}