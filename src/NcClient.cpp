/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   NcClient.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bvaujour <bvaujour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/31 15:24:30 by bvaujour          #+#    #+#             */
/*   Updated: 2024/05/31 17:37:17 by bvaujour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../NcClient.hpp"

NcClient::NcClient()
{

}

NcClient::~NcClient()
{
}

NcClient::NcClient(const NcClient& toCpy)
{
	*this = toCpy;
}

NcClient&	NcClient::operator=(const NcClient& toCpy)
{
	std::cout << GREEN << "NcClient assignement operator called" << RESET << std::endl;
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

void	NcClient::parseInput(std::string input)
{
	std::istringstream			iss(input);
	std::string					str;
	std::vector<std::string>	cmds;

	while (iss >> str)
		cmds.push_back(str);
	
}