/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mablatie <mablatie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/22 16:23:16 by vanitas           #+#    #+#             */
/*   Updated: 2024/06/03 17:52:30 by mablatie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../Client.hpp"
# include "../Server.hpp"


Client::Client() : _server(NULL)
{
	_isConnected = false;
	_passIsSet = false;
	_nickIsSet = false;
}

Client::Client(Server& server) : _server(&server)
{
	_isConnected = false;
	_passIsSet = false;
	_nickIsSet = false;
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
		this->_fd = rhs.getFd();
		this->_ip_addr = rhs.getIp();
		this->_username = rhs.getUsername();
		this->_channel = rhs.getChannel();
		this->_nick = rhs.getNick();
		this->_realname = rhs.getRealname();
		this->_nickIsSet = rhs._nickIsSet;
		this->_passIsSet = rhs._passIsSet;
		this->_isConnected = rhs._isConnected;
		this->_server = rhs._server;
	}
	return *this;
}

int	Client::getFd() const
{
	return this->_fd;
}

void	Client::setFd(int fd_input)
{
	this->_fd = fd_input;
}

const std::string& Client::getIp() const
{
	return this->_ip_addr;
}

void Client::setIp(int ip_input)
{
	this->_ip_addr = ip_input;
}

const std::string& Client::getUsername() const
{
	return this->_username;
}

void Client::setUsername(const std::string& user_input)
{
	this->_username = user_input;
}

const std::string& Client::getRealname() const
{
	return this->_realname;
}

void Client::setRealname(const std::string& realname_input)
{
	this->_realname = realname_input;
}

const std::string& Client::getNick() const
{
	return this->_nick;
}

void Client::setNick(const std::string& nick_input)
{
	if (!_server->checkAllNicknames(nick_input))
	{
		std::cout << "SetNick refused nick" << std::endl;
		return ;
	}
	std::cout << "Nick Set to " << nick_input << std::endl;
	_nickIsSet = true;
	Answer(user_id(this->getNick(), this->getUsername()) + " NICK :" + nick_input + "\r\n");
	this->_nick = nick_input;
}


const std::string& Client::getChannel() const
{
	return this->_channel;
}

void Client::setChannel(const std::string& channel_input)
{
	this->_channel = channel_input;
}

const std::string& Client::getPass() const
{
	return this->_pass;
}

void Client::setPass(const std::string& pass_input)
{
	if (pass_input != _server->getPassword())
	{
		std::cout << "SetPass refused password" << std::endl;
		return ;
	}
	std::cout << "pass Set to " << pass_input << std::endl;
	_passIsSet = true;
	this->_pass = pass_input;
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

std::vector<std::string>	Client::splitInput(const std::string& input) //static
{
	std::istringstream					iss(input);
	std::string							str;
	std::vector<std::string>			split;

	while (iss >> str)
		split.push_back(str);
	return (split);
}


void		Client::Answer(const std::string& answer)
{
	
	send(getFd(),answer.c_str(), answer.length(), 0);
	std::cout << CYAN << "[Server send]" << answer << RESET << std::endl;
}

void	Client::ParseAndRespond(std::string& input)
{
	Destination							dest;
	std::vector<std::string>			cmds;
	std::vector<std::string>::iterator	it;
	std::string							rep;
	size_t								nl_pos;

	dest = DEFAULT;
	_message += input;
	nl_pos = _message.find('\n');
	std::cout << "message en cours" << std::endl;
	if (nl_pos != _message.npos)
	{
		std::cout << "message finished" << std::endl;
		cmds = Client::splitInput(_message);
		it = std::find(cmds.begin(), cmds.end(), "USER");
		if (it != cmds.end() && it + 1 != cmds.end())
			setUsername(*(it + 1));
		it = std::find(cmds.begin(), cmds.end(), "NICK");
		if (it != cmds.end() && it + 1 != cmds.end())
			setNick(*(it + 1));
		it = std::find(cmds.begin(), cmds.end(), "PASS");
		if (it != cmds.end() && it + 1 != cmds.end())
			setPass(*(it + 1));
		it = std::find(cmds.begin(), cmds.end(), "PING");
		if (it != cmds.end() && it + 1 != cmds.end())
			Answer("PONG :" + *(it + 1) + "\r\n");
		it = std::find(cmds.begin(), cmds.end(), "PRIVMSG");
		if (it != cmds.end() && it + 1 != cmds.end())
		{
			input = ":" + _nick + " :" + _message;
			dest = SEND_CHAN;
		}
		if (_nickIsSet && _passIsSet && !_isConnected)
		{
			_isConnected = true;
			Answer(msg_serv(std::string("001"), this->getNick()) + GREEN + "Welcome to the IRC server\r\n"
				+ msg_serv(std::string("002"), this->getNick()) + GREEN + "Your host is 42IRCserver, running version 0.1\r\n"
				+ msg_serv(std::string("003"), this->getNick()) + GREEN + "This server was created " + _server->getDate() + "\r\n"
				+ msg_serv2(std::string("004"), this->getNick()) + GREEN + "42IRCserver 0.1 connected \r\n"
				+ msg_serv3(std::string("005"), this->getNick(), supp_info()));
		}
		_message.clear();
	}
}

ADNSANDFSNHPFJDSNfjrsdnfgrwmforsdwmGfrkgrs';