/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerUtils.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mablatie <mablatie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/27 14:28:29 by bvaujour          #+#    #+#             */
/*   Updated: 2024/06/03 16:07:02 by mablatie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Server.hpp"

void	Server::clearClient(Client& client)
{
	std::vector<Client>::iterator			it;
	std::vector<struct pollfd>::iterator	it2;

	std::cout << "Entering ClearClient" << std::endl;
	it2 = _pfds.begin();
	while (it2 != _pfds.end() && it2->fd != client.getFd())
		it2++;
	_pfds.erase(it2);
	it = _Clients.begin();
	while (it != _Clients.end() && it->getFd() != client.getFd())
		it++;
	close(it->getFd());
	_Clients.erase(it);
}

void Server::signalHandler(int signum) //static
{
	(void)signum;
	std::cout << "SIGNAL RECEIVED" << std::endl;
	Server::_signal = true;
}

void Server::sendWithCode(const Client& client, const std::string& code, const std::string& msg, const std::string& color) const
{
	std::string message = ":42IRCserver " + code + " " + client.getNick() + " :" + color + msg + "\r\n";
	send(client.getFd(), message.c_str(), message.length(), 0);
	std::cout << CYAN << "[Server send]" << message << RESET << std::endl;
}

void		Server::detailString(const std::string& str) const
{
	int i(0);

	std::cout << YELLOW << "detailString:";
	while (str[i])
	{
		if (str[i] == '\n')
			std::cout << "\\n";
		else if (str[i] == '\r')
			std::cout << "\\r";
		else
			std::cout << str[i];
		i++;
	}
	std::cout << RESET << std::endl;;

}
void Server::sendBasic(const Client& client, const std::string& msg, const std::string& color) const
{
	std::string message = color + msg + "\r\n";
	send(client.getFd(), message.c_str(), message.length(), 0);
	std::cout << CYAN << "[Server send]" << message << RESET << std::endl;
}

void Server::getServerCreationTime()
{
    // Obtenir l'heure actuelle
    std::time_t now = std::time(nullptr);
    // Convertir l'heure en structure tm pour le formatage
    std::tm* now_tm = std::localtime(&now);
    
    // Créer une chaîne de caractères pour la date et l'heure formatées
    char buffer[80] = {0};
    std::strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", now_tm);
    
    // Retourner la chaîne formatée
    this->_date = buffer;
}