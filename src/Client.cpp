# include "../Client.hpp"
# include "../Channel.hpp"
# include "../Server.hpp"


Client::Client() : _server(NULL)
{
	_isConnected = false;
	_passIsSet = false;
	_nickIsSet = false;
	_fd = -1;
	setLG((t_LoupGarouPlayerID){0, "none", 0, false, false, false, false,0});
}

Client::Client(Server& server) : _server(&server)
{
	_isConnected = false;
	_passIsSet = false;
	_nickIsSet = false;
	_fd = -1;
	setLG((t_LoupGarouPlayerID){0, "none", 0, false, false, false, false,0});
}

Client::~Client()
{
	std::cout << RED << "Client destructed" << RESET << std::endl;
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
		this->_nick = rhs._nick;
		this->_realname = rhs._realname;
		this->_nickIsSet = rhs._nickIsSet;
		this->_passIsSet = rhs._passIsSet;
		this->_isConnected = rhs._isConnected;
		this->_server = rhs._server;
		this->_message = rhs._message;
		this->_inChannels = rhs._inChannels;
		this->_inChannels = rhs._inChannels;
		this->_InvitedAt = rhs._InvitedAt;
		this->_LG_PlayerID = rhs._LG_PlayerID;
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

void	Client::setOPChannels(Channel* chan)
{
	this->_OPChannels.push_back(chan);
}

void	Client::setLG(const t_LoupGarouPlayerID& Player_ID)
{
	this->_LG_PlayerID = Player_ID;
}

t_LoupGarouPlayerID&		Client::getLG()
{
	return (this->_LG_PlayerID);
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
	pos = input.find_last_not_of("\r\n");
    if (pos != std::string::npos)
        input.erase(pos + 1);
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

const std::string Client::PickMsg(const std::string& input)
{
    size_t pos = input.find(':');

    if (pos != std::string::npos)
        return input.substr(pos + 1);
    else
        return "";
}

void	Client::channelThrow(const std::string& channelName)
{
	Channel* channel;
	channel = _server->findChannel(channelName);

	if (channelName[0] != '#')
		throw(Error("error: Invalid channel key/user mode not supported"));
	if (channel == NULL)
	{
		FormatIRC::sendCodeMsg(*this, "401", channelName , "No such nick/channel");
		throw(Error("error: channel does not exist"));
	}
}

void	Client::operatorThrow(const std::string& channelName)
{
	Channel* channel;
	channel = _server->findChannel(channelName);

	if (!channel->IsAnOp(this->_nick))
	{
		FormatIRC::sendCodeMsg(*this, "482", channelName , "You're not channel operator");
		throw(Error("error: not operator"));
	}
}

void	Client::beInvited(Channel *channel)
{
	std::vector<Channel*>::iterator it;

	it = std::find(_InvitedAt.begin(), _InvitedAt.end(), channel);
	if (it == _InvitedAt.end())
		_InvitedAt.push_back(channel);
}