/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ClientCore.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bvaujour <bvaujour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/28 10:03:14 by bvaujour          #+#    #+#             */
/*   Updated: 2024/05/30 17:28:32 by bvaujour         ###   ########.fr       */
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
    // smiley(input);
    for (std::vector<Client>::iterator it = clients.begin(); it < clients.end(); it++)
    {
        if (it->getFd() != fd && it->getState() == CONNECTED)
        {
			std::cout << input.c_str() << std::endl;
            // it->printPrompt(std::string(":chuck 001 user:") + input.c_str());
            it->printPrompt(input.c_str());
        }
    }
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
