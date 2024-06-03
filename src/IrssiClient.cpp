/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IrssiClient.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bvaujour <bvaujour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/31 15:09:29 by bvaujour          #+#    #+#             */
/*   Updated: 2024/06/02 19:41:09 by bvaujour         ###   ########.fr       */
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

	std::cout << GREEN << "IRSSI client created" << RESET << std::endl;
	_fd = fd;
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
void		IrssiClient::formatText(std::string& input)
{
	(void)input;
}
Destination	IrssiClient::ParseAndRespond(std::string& input)
{
	std::vector<std::string>			cmds;
	std::vector<std::string>::iterator	it;
	std::string							rep;
	size_t								nl_pos;

	_message += input;
	nl_pos = _message.find("\r\n");
	if (nl_pos != _message.npos)
	{
		_message.erase(nl_pos);
		cmds = Client::splitInput(_message);
		it = std::find(cmds.begin(), cmds.end(), "PING");
		if (it != cmds.end() && it + 1 != cmds.end())
		{
			input = "PONG :" + *(it + 1);
			_message.clear();
			return (ANSWER_SENDER);
		}
		it = std::find(cmds.begin(), cmds.end(), "PRIVMSG");
		if (it != cmds.end() && it + 1 != cmds.end())
		{
			input = ":" + _nick + " :" + _message;
			_message.clear();
			return (ANSWER_SENDER);
		}
		_message.clear();
	}	
	return (DEFAULT);
}