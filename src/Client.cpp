/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bvaujour <bvaujour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/22 16:23:16 by vanitas           #+#    #+#             */
/*   Updated: 2024/05/31 17:50:18 by bvaujour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Client.hpp"

Client::Client()
{
	
}

Client::~Client()
{
}

Client::Client(const Client& cpy)
{
	*this = cpy;
	std::cout << GREEN << "Client copy constructor called" << RESET << std::endl;
}

Client& Client::operator=(const Client& rhs)
{
	std::cout << GREEN << "Client assignement operator called" << RESET << std::endl;
	if (this != &rhs)
	{
		this->_fd = rhs.getFd();
		this->_ip_addr = rhs.getIp();
		this->_username = rhs.getUsername();
		this->_channel = rhs.getChannel();
		this->_state = rhs.getState();
	}
	return *this;
}

int Client::getFd() const
{
	return this->_fd;
}

void Client::setFd(int fd_input)
{
	this->_fd = fd_input;
}

const std::string& Client::getIp() const
{
	return this->_ip_addr;
}

void Client::setIp(int ip_input)
{
	this->_ip_addr = ip_input;
}

const std::string& Client::getUsername() const
{
	return this->_username;
}

void Client::setUsername(std::string user_input)
{
	this->_username = user_input;
}


const std::string& Client::getChannel() const
{
	return this->_channel;
}

void Client::setChannel(std::string channel_input)
{
	this->_channel = channel_input;
}

const State& Client::getState() const
{
	return this->_state;
}

void Client::setState(const State& state_input)
{
	this->_state = state_input;
}

const std::string& Client::getPass() const
{
	return this->_pass;
}

void Client::setPass(std::string pass_input)
{
	this->_pass = pass_input;
}

void    Client::smiley(std::string& input)
{
    size_t    pos;

    pos = input.find("=)", 0);
    if (pos != input.npos)
    {
        input.erase(pos, 2);
        input.insert(pos, SMILE);
    }
        
}

void	Client::parseInput(std::string input)
{
	(void)input;
}
