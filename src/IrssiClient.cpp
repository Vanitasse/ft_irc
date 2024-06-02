/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IrssiClient.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bvaujour <bvaujour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/31 15:09:29 by bvaujour          #+#    #+#             */
/*   Updated: 2024/06/02 02:25:58 by bvaujour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../IrssiClient.hpp"

IrssiClient::IrssiClient()
{
	std::cout << GREEN << "IRSSI client created" << RESET << std::endl;
}

IrssiClient::~IrssiClient()
{
	std::cout << RED << "IRSSI Client destructed" << RESET << std::endl;
}

IrssiClient::IrssiClient(const IrssiClient& toCpy)
{
	std::cout << GREEN << "IRSSI client copied" << RESET << std::endl;
	*this = toCpy;
}

IrssiClient&	IrssiClient::operator=(const IrssiClient& toCpy)
{
	std::cout << GREEN << "IrssiClient assignement operator called" << RESET << std::endl;
	if (this != &toCpy)
	{
		this->_fd = toCpy.getFd();
		this->_ip_addr = toCpy.getIp();
		this->_username = toCpy.getUsername();
		this->_channel = toCpy.getChannel();
		this->_state = toCpy.getState();
	}
	return (*this);
}

IrssiClient::IrssiClient(const std::string& input, int fd)
{
	std::vector<std::string>			infos;
	std::vector<std::string>::iterator	it;

	setFd(fd);
	infos = Client::splitInput(input);
	it = std::find(infos.begin(), infos.end(), "USER");
	if (it != infos.end() && it + 1 != infos.end())
		_username = *(it + 1);
	it = std::find(infos.begin(), infos.end(), "NICK");
	if (it != infos.end() && it + 1 != infos.end())
		_nick = *(it + 1);
	it = std::find(infos.begin(), infos.end(), "PASS");
	if (it != infos.end() && it + 1 != infos.end())
		_pass = *(it + 1);

}

void	IrssiClient::ParseAndRespond(std::string input)
{
	std::vector<std::string>			cmds;
	std::vector<std::string>::iterator	it;
	std::string							rep;

	cmds = Client::splitInput(input);
	it = std::find(cmds.begin(), cmds.end(), "PING");
	if (it != cmds.end() && it + 1 != cmds.end())
	{
		rep = "PONG :" + *(it + 1) + "\r\n";
		send(_fd, rep.c_str(), rep.size(), 0);
		std::cout << CYAN << "[Server send]" << rep << RESET << std::endl;
	}
}