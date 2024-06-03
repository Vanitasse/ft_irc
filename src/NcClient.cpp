/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   NcClient.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bvaujour <bvaujour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/31 15:24:30 by bvaujour          #+#    #+#             */
/*   Updated: 2024/06/02 19:40:12 by bvaujour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../NcClient.hpp"

NcClient::NcClient()
{
	std::cout << GREEN << "NC Client created" << RESET << std::endl;
}

NcClient::~NcClient()
{
	std::cout << RED << "NC Client destructed" << RESET << std::endl;
}

NcClient::NcClient(const NcClient& toCpy)
{
	std::cout << GREEN << "NC Client copied" << RESET << std::endl;
	*this = toCpy;
}

NcClient&	NcClient::operator=(const NcClient& toCpy)
{
	std::cout << GREEN << "NC Client assignement operator called" << RESET << std::endl;
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

void		NcClient::formatText(std::string& input)
{
	(void)input;
}
Destination	NcClient::ParseAndRespond(std::string& input)
{
	std::vector<std::string>	cmds;
	size_t	nl_pos;

	_message += input;
	nl_pos = _message.find('\n');
	if (nl_pos != _message.npos)
	{
		_message.erase(nl_pos);
		cmds = Client::splitInput(_message);
		input = _message;
		_message.clear();
		return (SEND_CHAN);
	}
	return (DEFAULT);
}