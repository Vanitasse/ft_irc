/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   NcClient.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bvaujour <bvaujour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/31 15:24:30 by bvaujour          #+#    #+#             */
/*   Updated: 2024/06/02 12:41:42 by bvaujour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../NcClient.hpp"

NcClient::NcClient()
{
	std::cout << GREEN << "Nc Client created" << RESET << std::endl;
}

NcClient::~NcClient()
{
	std::cout << RED << "Nc Client destructed" << RESET << std::endl;
}

NcClient::NcClient(const NcClient& toCpy)
{
	std::cout << GREEN << "Nc Client copied" << RESET << std::endl;
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

Response	NcClient::ParseAndRespond(std::string& input)
{
	std::vector<std::string>	cmds;

	cmds = Client::splitInput(input);
	return (DEFAULT);
}