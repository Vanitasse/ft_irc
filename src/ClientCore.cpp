/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ClientCore.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bvaujour <bvaujour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/28 10:03:14 by bvaujour          #+#    #+#             */
/*   Updated: 2024/05/28 12:57:41 by bvaujour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Client.hpp"

void	Client::loginRecv(std::string input)
{
	if (input.length() > 0)
		input[input.length() - 1] = '\0';
	username = input;
	state = PASSWORD;
	printPrompt("Enter Server password : ");
}

void	Client::passwordRecv(std::string input, const std::string& password)
{
	if (input == password + '\n')
	{
		send(this->fd, "Welcome!\n", 10, 0);
		state = CONNECTED;
		printPrompt(GREEN + std::string("#") + this->channel + ": " + RESET);
	}
	else
		printPrompt("Enter Server password : ");
}

void	Client::connectedRecv(std::string input, std::vector<Client> clients)
{
	std::string	prompt;

	for (std::vector<Client>::iterator it = clients.begin(); it < clients.end(); it++)
	{
		if (it->getFd() != fd && it->getState() == CONNECTED)
		{
			send(it->getFd(), "\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b", 20, 0);
			it->printPrompt(std::string(GREEN) + "#" + this->channel + "->" + BLUE + username + ": " + RESET);
			send(it->getFd(), input.c_str(), input.size(), 0);
			it->printPrompt(std::string(GREEN) + "#" + this->channel + ": " + RESET);
		}
	}
	printPrompt(GREEN + std::string("#") + this->channel + ": " + RESET);
}

void	Client::printPrompt(std::string prompt)
{
	send(fd, prompt.c_str(), prompt.size(), 0);
}
