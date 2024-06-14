# include "../Client.hpp"
# include "../Channel.hpp"
# include "../Server.hpp"

void	Client::NICK(const std::string& newName)
{
	if (!_server->checkNicks(newName))
	{
		std::cout << "SetNick refused nick" << std::endl;
		return ;
	}
	std::cout << "Nick Set to " << newName << std::endl;
	_nickIsSet = true;
	FormatIRC::sendNICK(this->_fd, *this, newName);
	setNick(newName);
}

void	Client::PASS(const std::string& password)
{
	if (password != _server->getPassword())
	{
		std::cout << "SetPass refused password" << std::endl;
		return ;
	}
	_passIsSet = true;
}

void	Client::JOIN(const std::string& channelName)
{
	Channel *JOINChannel;

	for (auto& channel : _inChannels)
		if (channel->getName() == channelName)
			return ;
	JOINChannel = _server->checkChannels(channelName, *this);
	if (JOINChannel->getModes()._k == true)
		return (FormatIRC::sendCodeMsg(*this, "475", channelName, "Cannot join channel (+k)"));
	if (JOINChannel == NULL)
	{
		FormatIRC::sendCodeMsg(*this, "448", channelName , "Cannot join channel: Channel name is too long");
		return ;
	}
	JOINChannel->addClient(this);
	JOINChannel->setTopic(JOINChannel->getTopic(), *this);
	_inChannels.push_back(JOINChannel);
	FormatIRC::sendJOIN(*this, *JOINChannel);
}

// :maxou!~mablatie@B2ED245D.B270E442.5F584402.IP JOIN :#aa
// >> :roubaix.fr.epiknet.org 353 maxou = #aa :maxou @max
// >> :roubaix.fr.epiknet.org 366 maxou #aa :End of /NAMES list.

// >> :maxz!~mablatie@B2ED245D.B270E442.5F584402.IP JOIN :#aa
// >> :max!~mablatie@B2ED245D.B270E442.5F584402.IP QUIT :Quit:: leaving


// void	Client::JOIN(const std::string& channelName)
// {
// 	Channel *JOINChannel;

void	Client::JOIN(const std::string& channelName, const std::string& password)
{
	Channel *JOINChannel;

	for (auto& channel : _inChannels)
		if (channel->getName() == channelName)
			return ;
	JOINChannel = _server->checkChannels(channelName, *this);
	if (JOINChannel->getModes()._k == true && JOINChannel->getPassword() != password)
		return (FormatIRC::sendCodeMsg(*this, "475", channelName, "Cannot join channel (+k)"));
	if (JOINChannel == NULL)
	{
		FormatIRC::sendCodeMsg(*this, "448", channelName , "Cannot join channel: Channel name is too long");
		return ;
	}
	JOINChannel->addClient(this);
	JOINChannel->setTopic(JOINChannel->getTopic(), *this);
	_inChannels.push_back(JOINChannel);
	FormatIRC::sendJOIN(*this, *JOINChannel);
}

void	Client::PRIVMSG(const std::string& destination, const std::string& msg)
{
	std::vector<Channel*>::iterator	it;

	it = _inChannels.begin();
	while (it != _inChannels.end() && (*it)->getName() != destination)
		it++;
	if (it == _inChannels.end())
	{
		std::cout << RED << "client " << _nick << " try to talk to a channel but is not inside" << RESET << std::endl;
		return ;
	}
	(*it)->sendToClients(*this, msg);
}

void	Client::QUIT()
{
	FormatIRC::sendQUIT(*this);
	for (std::vector<Channel*>::iterator it = _inChannels.begin(); it != _inChannels.end(); it++)
	{
		(*it)->removeOP(*this);
		(*it)->removeClient(this);
	}
	_server->clearClient(this);
}

void	Client::PART(const std::string& channelName, const std::string& partMsg)
{
	for (std::vector<Channel*>::iterator it = _inChannels.begin(); it != _inChannels.end(); it++)
		if (channelName == (*it)->getName())
		{
			FormatIRC::sendPART(*this, channelName, partMsg, (*it)->getChanClients());
			(*it)->removeClient(this);
			this->_inChannels.erase(it);
			break ;
		}
}

void	Client::KICK(const std::string & channelName, const std::string& user_kicked, const std::string& reason)
{
	std::vector<Client*> sndclients;
	Client* client_kicked;

	channelThrow(channelName);
	operatorThrow(channelName);
	if (this->getNick() == user_kicked)
		return;
	client_kicked = _server->findClient(user_kicked);
	if (!client_kicked)
		return;
	for(std::vector<Channel*>::iterator it = _OPChannels.begin(); it < _OPChannels.end(); it++)
	{
		if ((*it)->getName() == channelName)
		{
			client_kicked->PART(channelName, RED + std::string("IM KICKED BYE") + RESET);
			(*it)->removeClient(user_kicked);
			sndclients = (*it)->getChanClients();
			break;

		}
	}
	FormatIRC::sendKICK(*this, channelName, user_kicked, sndclients, reason);
}

void	Client::MODE(const std::string& channelName, const std::string& mode)
{
	Channel*	channel;

	channelThrow(channelName);
	operatorThrow(channelName);
	channel = _server->findChannel(channelName);
	if (mode[0] == '+')
		switch (mode[1])
		{
			case 't':
				if (channel->enable_T(true))
				FormatIRC::sendMODE(*this, channelName, "+t", channel->getChanClients());
				break;
			default:
				break;
		}
	else if (mode[0] == '-')
		switch (mode[1])
		{
			case 'k':
				if (channel->enable_K(false, channel->getPassword()))
					FormatIRC::sendMODE(*this, channelName, std::string("-k") + " " + channel->getPassword(), channel->getChanClients()); // arg
				break;
			case 'l':
				if (channel->enable_L(0))
					FormatIRC::sendMODE(*this, channelName, std::string("-l") + " " + channel->getPassword(), channel->getChanClients()); // arg
				break;
			case 't':
				if (channel->enable_T(false))
					FormatIRC::sendMODE(*this, channelName, "+t", channel->getChanClients());
				break;
			default:
				break;
		}
}

void	Client::MODE(const std::string& channelName, const std::string& mode, const std::string& arg)
{
	Channel*	channel;

	channelThrow(channelName);
	operatorThrow(channelName);
	channel = _server->findChannel(channelName);

	if (mode[0] == '+')
		switch (mode[1])
		{
			case 'k':
				if (channel->enable_K(true, arg))
				FormatIRC::sendMODE(*this, channelName, std::string("+k") + " " + arg, channel->getChanClients());
				break ;
			case 'o':
				if (!channel->IsAnOp(arg))
				{
					if (channel->IsInChan(arg))
					{
						FormatIRC::sendMODE(*this, channelName, std::string("+o") + " " + arg, channel->getChanClients());
						channel->addOperator(_server->findClient(arg));
					}
					else
						FormatIRC::sendCodeMsg(*this, "401", channelName , "No such nick/channel");
					break ;
				}
			case 'l':
				for (std::size_t i = 0; i < arg.length(); i++)
					if (!std::isdigit(arg[i]))
						return (FormatIRC::sendCodeMsg(*this, "349", channelName, "Invalid Arg (+l)"));
				if (std::stoull(arg) > std::numeric_limits<std::size_t>::max())
				{
					FormatIRC::sendCodeMsg(*this, "349", channelName, "Invalid Arg (+l)");
					throw(std::out_of_range("La valeur dépasse les limites de size_t"));
				}
				else if (channel->enable_L(std::stoull(arg)))
				{
					if (std::stoull(arg) == 0)
						FormatIRC::sendMODE(*this, channelName, std::string("-l"), channel->getChanClients());
					else
						FormatIRC::sendMODE(*this, channelName, std::string("+l") + " " + arg, channel->getChanClients());
				}
				break ;
			default:
				break;
		}
	else if (mode[0] == '-')
		switch (mode[1])
		{
			case 'k':
				if (channel->enable_K(false, arg))
					FormatIRC::sendMODE(*this, channelName, std::string("-k") + " " + arg, channel->getChanClients());
				break;
			case 'o':
				if (channel->IsAnOp(arg))
				{
					FormatIRC::sendMODE(*this, channelName, std::string("-o") + " " + arg, channel->getChanClients());
					channel->removeOP(*_server->findClient(arg));
				}
				break ;
			default:
				break;
		}
}

void	Client::TOPIC(const std::string& channelName, const std::string& topic)
{
	Channel	*channel;

	channelThrow(channelName);
	channel = _server->findChannel(channelName);
	if (channel->getModes()._t == true)
		operatorThrow(channelName);
	channel->setTopic(topic, *this);
	FormatIRC::updateTOPIC(*this, channel);
}

void	Client::ParseAndRespond(std::string& input)
{
	std::vector<std::string>			cmds;
	std::vector<std::string>::iterator	it;

	_message += input;
	if (_message.find('\n') != _message.npos)
	{
		try
		{
			smiley(_message);
			cmds = Client::splitInput(_message);
			it = std::find(cmds.begin(), cmds.end(), "QUIT");
			if (it != cmds.end() && it + 1 != cmds.end())
				return (QUIT());

			it = std::find(cmds.begin(), cmds.end(), "USER");
			if (it != cmds.end() && it + 1 != cmds.end())
				setUsername(*(it + 1));

			it = std::find(cmds.begin(), cmds.end(), "NICK");
			if (it != cmds.end() && it + 1 != cmds.end())
				NICK(*(it + 1));

			it = std::find(cmds.begin(), cmds.end(), "PASS");
			if (it != cmds.end() && it + 1 != cmds.end())
				PASS(*(it + 1));

			it = std::find(cmds.begin(), cmds.end(), "PING");
			if (it != cmds.end() && it + 1 != cmds.end())
				FormatIRC::sendPONG(this->_fd, *(it + 1));

			it = std::find(cmds.begin(), cmds.end(), "PRIVMSG");
			if (it != cmds.end() && it + 1 != cmds.end() && it + 2 != cmds.end())
				PRIVMSG(*(it + 1), PickMsg(_message));

			it = std::find(cmds.begin(), cmds.end(), "PART");
			if (it != cmds.end() && it + 1 != cmds.end() && it + 2 != cmds.end())
				PART(*(it + 1), PickMsg(_message));
			else if (it != cmds.end() && it + 1 != cmds.end())
				PART(*(it + 1), "");
			it = std::find(cmds.begin(), cmds.end(), "MODE");
			if (it != cmds.end() && it + 1 != cmds.end() && it + 2 != cmds.end() && it + 3 != cmds.end())
				MODE(*(it + 1), *(it + 2), *(it + 3));
			else if (it != cmds.end() && it + 1 != cmds.end() && it + 2 != cmds.end())
				MODE(*(it + 1), *(it + 2));


			it = std::find(cmds.begin(), cmds.end(), "TOPIC");
			if (it != cmds.end() && it + 1 != cmds.end() && it + 2 != cmds.end())
				TOPIC(*(it + 1), PickMsg(_message));

			it = std::find(cmds.begin(), cmds.end(), "KICK");
			if (it != cmds.end() && it + 1 != cmds.end() && it + 2 != cmds.end() && it + 3 != cmds.end())
				KICK(*(it + 1), *(it + 2), *(it + 3));
			else if (it != cmds.end() && it + 1 != cmds.end() && it + 2 != cmds.end())
				KICK(*(it + 1), *(it + 2), "");

			it = std::find(cmds.begin(), cmds.end(), "JOIN");
			if (it != cmds.end() && it + 1 != cmds.end() && it + 2 != cmds.end())
				JOIN(*(it + 1), *(it + 2));
			else if (it != cmds.end() && it + 1 != cmds.end())
				JOIN(*(it + 1));
			if (!_isConnected && _nickIsSet && _passIsSet)
			{
				_isConnected = true;
				FormatIRC::sendWelcome(this->_fd, this->_nick, _server->getDate());
			}
		}
		catch(const std::exception& e)
		{
			std::cerr << RED << e.what() << RESET << std::endl;
		}
		_message.clear();
	}
}