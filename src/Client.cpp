# include "../Client.hpp"
# include "../Channel.hpp"
# include "../Server.hpp"


Client::Client() : _server(NULL)
{
	_isConnected = false;
	_passIsSet = false;
	_nickIsSet = false;
	_fd = -1;
}

Client::Client(Server& server) : _server(&server)
{
	_isConnected = false;
	_passIsSet = false;
	_nickIsSet = false;
	_fd = -1;
}

Client::~Client()
{
}

Client::Client(const Client& cpy)
{
	*this = cpy;
	std::cout << GREEN << "Client copy constructor called" << RESET << std::endl;
}

Client& Client::operator=(const Client& rhs)
{
	std::cout << GREEN << "Client assignement operator called" << RESET << std::endl;
	if (this != &rhs)
	{
		this->_fd = rhs._fd;
		this->_ip_addr = rhs._ip_addr;
		this->_username = rhs._username;
		this->_inChannels = rhs._inChannels;
		this->_nick = rhs._nick;
		this->_realname = rhs._realname;
		this->_nickIsSet = rhs._nickIsSet;
		this->_passIsSet = rhs._passIsSet;
		this->_isConnected = rhs._isConnected;
		this->_server = rhs._server;
	}
	return *this;
}

int	Client::getFd() const
{
	return (this->_fd);
}

void	Client::setFd(int fd_input)
{
	this->_fd = fd_input;
}

const std::string& Client::getIp() const
{
	return (this->_ip_addr);
}

void Client::setIp(int ip_input)
{
	this->_ip_addr = ip_input;
}

const std::string& Client::getUsername() const
{
	return (this->_username);
}

void Client::setUsername(const std::string& user_input)
{
	this->_username = user_input;
}

const std::string& Client::getRealname() const
{
	return (this->_realname);
}

void Client::setRealname(const std::string& realname_input)
{
	this->_realname = realname_input;
}

const std::string& Client::getNick() const
{
	return (this->_nick);
}

void Client::setNick(const std::string& nick_input)
{
	this->_nick = (nick_input);
}


const std::vector<Channel*>& Client::getInChannels() const
{
	return (this->_inChannels);
}

const bool&	Client::getPassIsSet() const
{
	return (_passIsSet);
}

void 	Client::setPassIsSet(const bool& passIsSet_input)
{
	_passIsSet = passIsSet_input;
}

const bool&	Client::getNickIsSet() const
{
	return (_nickIsSet);
}

void 	Client::setNickIsSet(const bool& nickIsSet_input)
{
	_nickIsSet = nickIsSet_input;
}

const bool&	Client::getIsConnected() const
{
	return (_passIsSet);
}

void 	Client::setIsConnected(const bool& isConnected_input)
{
	_isConnected = isConnected_input;
}

void	Client::smiley(std::string& input)
{
    size_t    pos;

    pos = input.find("=)", 0);
    if (pos != input.npos)
    {
        input.erase(pos, 2);
        input.insert(pos, SMILE);
    }
}

std::vector<std::string>	Client::splitInput(const std::string& input) //static
{
	std::istringstream					iss(input);
	std::string							str;
	std::vector<std::string>			split;

	while (iss >> str)
		split.push_back(str);
	return (split);
}


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
	JOINChannel = _server->checkChannels(channelName);
	if (JOINChannel == NULL)
	{
		FormatIRC::sendErrorChannelLen(*this, channelName , _server->getDomain());
		return ;
	}
	JOINChannel->addClient(this);
	JOINChannel->setTopic("", *this);
	_inChannels.push_back(JOINChannel);
	FormatIRC::sendJOIN(*this, *JOINChannel , _server->getDomain());
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
	FormatIRC::sendQUIT(this->_fd, this->getNick(), this->getUsername());
	for (std::vector<Channel*>::iterator it = _inChannels.begin(); it != _inChannels.end(); it++)
		(*it)->channelClearClient(this);
	_server->clearClient(this);
}

void	Client::PART(const std::string& channelName, const std::string& partMsg)
{
	for (std::vector<Channel*>::iterator it = _inChannels.begin(); it != _inChannels.end(); it++)
		if (channelName == (*it)->getName())
		{
			(*it)->sendToClients(*this, partMsg);
			FormatIRC::sendPART(*this, channelName, partMsg);
			_inChannels.erase(it);
			break ;
		}
}


void	Client::ParseAndRespond(std::string& input)
{
	std::vector<std::string>			cmds;
	std::vector<std::string>::iterator	it;
	std::string							rep;
	size_t								nl_pos;

	_message += input;
	nl_pos = _message.find('\n');
	std::cout << "message en cours" << std::endl;
	if (nl_pos != _message.npos)
	{
		std::cout << "message finished" << std::endl;
		cmds = Client::splitInput(_message);
		it = std::find(cmds.begin(), cmds.end(), "QUIT");
		if (it != cmds.end() && it + 1 != cmds.end())
		{
			QUIT();
			return;
		}
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
			PRIVMSG(*(it + 1), *(it + 2));
		it = std::find(cmds.begin(), cmds.end(), "PART");
		if (it != cmds.end() && it + 1 != cmds.end() && it + 2 != cmds.end())
			PART(*(it + 1), *(it + 2));



		it = std::find(cmds.begin(), cmds.end(), "TOPIC");
		if (it != cmds.end() && it + 1 != cmds.end() && it + 2 != cmds.end())
			_server->TOPIC_2(*this, *(it + 1), *(it + 2));
		else if (it != cmds.end() && it + 1 != cmds.end())
			_server->TOPIC_1(*this, *(it + 1));


		// it = std::find(cmds.begin(), cmds.end(), "KICK");
		// if (it != cmds.end() && it + 1 != cmds.end() && it + 2 != cmds.end() && it + 3 != cmds.end())
		// 	_server->KICK();
		// else if (it != cmds.end() && it + 1 != cmds.end() && it + 2 != cmds.end())
		// 	_server->KICK();

		// else if (it != cmds.end() && it + 1 != cmds.end())
		// 	_server->KICK();



		if (!_isConnected && _nickIsSet && _passIsSet)
		{
			_isConnected = true;
			FormatIRC::sendWelcome(this->_fd, this->_nick, _server->getDate());
		}
		it = std::find(cmds.begin(), cmds.end(), "JOIN");
		if (it != cmds.end() && it + 1 != cmds.end())
			JOIN(*(it + 1));
		_message.clear();
	}
}