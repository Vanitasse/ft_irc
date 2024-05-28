/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ClientCore.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mablatie <mablatie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/28 10:03:14 by bvaujour          #+#    #+#             */
/*   Updated: 2024/05/28 16:58:24 by mablatie         ###   ########.fr       */
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
		printPrompt(GREEN + this->channel + ": " + RESET);
	}
	else
		printPrompt("Enter Server password : ");
}

void    Client::connectedRecv(std::string input, std::vector<Client> &clients)
{
    smiley(input);
    for (std::vector<Client>::iterator it = clients.begin(); it < clients.end(); it++)
    {
        if (it->getFd() != fd && it->getState() == CONNECTED && this->channel == it->channel)
        {
            it->printPrompt("\r" + std::string(GREEN) + this->channel + "->" + BLUE + username + ": " + RESET + input.c_str());
            it->printPrompt(std::string(GREEN) + it->getChannel() + ": " + RESET);
        }
    }
    printPrompt(GREEN + this->channel + ": " + RESET);
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

void	Client::printPrompt(std::string prompt)
{
	send(fd, prompt.c_str(), prompt.size(), 0);
}
