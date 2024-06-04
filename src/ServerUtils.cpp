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
	it = _Clients.begin() + 1; // on check pas le bot
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


Channel&	Server::newChannelAccess(const std::string& chanName)
{
	std::vector<Channel>::iterator	it;
	Channel newChannel;

	std::cout << "newChannelAccess look for channel " << chanName << std::endl;
	it = _Channels.begin();
	while (it != _Channels.end())
	{
		if (it->getName() == chanName)
		{
			std::cout << chanName << " exists, returning it" << std::endl;
			return (*it);
		}
		it++;
	}
	newChannel.setName(chanName);
	_Channels.push_back(newChannel);
	std::cout << chanName << " created" << std::endl;
	return (*(_Channels.end() - 1));
}

int	Server::newNickAccess(const std::string& nickname)
{
	std::vector<Client>::iterator	it;

	it = _Clients.begin();
	while (it != _Clients.end())
	{
		if (it->getNick() == nickname)
			return (0);
		it++;
	}
	return (1);
}