/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IrssiClient.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bvaujour <bvaujour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/31 15:09:29 by bvaujour          #+#    #+#             */
/*   Updated: 2024/05/31 17:50:51 by bvaujour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../IrssiClient.hpp"

IrssiClient::IrssiClient()
{

}

IrssiClient::~IrssiClient()
{
}

IrssiClient::IrssiClient(const IrssiClient& toCpy)
{
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

void	IrssiClient::parseInput(std::string input)
{
	std::istringstream			iss(input);
	std::string					str;
	std::vector<std::string>	cmds;
	std::vector<std::string>::iterator	it;

	while (iss >> str)
		cmds.push_back(str);
	it = cmds.begin();
	while (it != cmds.end())
	{
		if (*it == "PASS")
			;
		it++;
	}
}