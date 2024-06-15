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

void Server::signalHandler(int signum)
{
	(void)signum;
	std::cout << "SIGNAL RECEIVED" << std::endl;
	Server::_signal = true;
}


void Server::getServerCreationTime()
{
	std::time_t now;
	std::tm* now_tm;
	char buffer[80] = {0};

	now = std::time(nullptr);
	now_tm = std::localtime(&now);
	std::strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", now_tm);
	this->_date = buffer;
}

Channel*	Server::createNewChannel(const std::string& chanName, Client& client)
{
	Channel *newChannel = new Channel();

	newChannel->setName(chanName);
	client.setOPChannels(newChannel);
	_Channels.push_back(newChannel);
	std::cout << chanName << " created" << std::endl;
	return (_Channels.back());
}

Channel*	Server::FindOrCreateChannel(const std::string& chanName, Client& client)
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
	return (createNewChannel(chanName, client));
}

Client*	Server::findClient(const std::string& nick)
{
	for(std::vector<Client*>::iterator it = _Clients.begin(); it < _Clients.end(); it++)
	{
		if ((*it)->getNick() == nick)
			return *it;
	}
	return nullptr;
}

Channel*	Server::findChannel(const std::string& channelName)
{
	for(std::vector<Channel*>::iterator it = _Channels.begin(); it < _Channels.end(); it++)
	{
		if ((*it)->getName() == channelName)
			return *it;
	}
	return nullptr;
}
