#include "../Server.hpp"

void	Server::clearClient(Client* client)
{
	std::cout << "Entering ClearClient to delete " << client->getNick() << " with fd " << client->getFd() << std::endl;
	for (std::vector<struct pollfd>::iterator	it = _pfds.begin(); it != _pfds.end(); it++)
		if (it->fd == client->getFd())
		{
			close(it->fd);
			_pfds.erase(it);
			break ;
		}
	for (std::vector<Client*>::iterator	it = _Clients.begin(); it != _Clients.end(); it++)
		if (*it == client)
		{
			delete (*it);
			*it = NULL;
			_Clients.erase(it);
			break ;
		}
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

Channel*	Server::createNewChannel(const std::string& chanName)
{
	Channel *newChannel = new Channel();

	newChannel->setName(chanName);
	_Channels.push_back(newChannel);
	std::cout << chanName << " created" << std::endl;
	return (_Channels.back());
}

Channel*	Server::checkChannels(const std::string& chanName)
{
	if (chanName.length() > _limits._channelLen)
		return (NULL);
	std::cout << "newChannelAccess look for channel " << chanName << std::endl;
	for (auto& channel : _Channels)
		if (channel->getName() == chanName)
		{
			std::cout << chanName << " exists, returning it" << std::endl;
			return (channel);
		}
	return (createNewChannel(chanName));
}

int	Server::checkNicks(const std::string& nickname)
{
	for (auto& client : _Clients)
		if (client->getNick() == nickname)
			return (0);
	return (1);
}