#include "../Channel.hpp"
#include <sys/time.h>

Channel::Channel()
{
	std::time_t currentTime = std::time(NULL);
	std::ostringstream oss;
	
	oss << currentTime;
	_date = oss.str();
	// _date = std::to_string(std::time(NULL));
	_userLimit = 0;
	_modes._i = false;
	_modes._t = false;
	_modes._k = false;
	_modes._l = false;
}

Channel::Channel(const Channel& cpy)
{
	*this = cpy;
}

Channel::~Channel() {}

Channel& Channel::operator=(const Channel& rhs)
{
	if (this != &rhs)
	{
		this->_modes = rhs._modes;
		this->_password = rhs._password;
		this->_userLimit = rhs._userLimit;
		this->_name = rhs._name;
		this->_who_topic = rhs._who_topic;
		this->_topic = rhs._topic;
		this->_operators = rhs._operators;
		this->_chanClients = rhs._chanClients;
		this->_date = rhs._date;
	}
	return *this;
}

const std::string& Channel::getName() const
{
	return (this->_name);
}

void Channel::setName(const std::string& name)
{
	this->_name = name;
}
const t_chanModes&	Channel::getModes() const
{
	return (this->_modes);
}
bool	Channel::enable_I(const bool i)
{
	if (_modes._i == i)
		return (false);
	_modes._i = i;
	return (true);
}
bool	Channel::enable_T(const bool t)
{
	if (_modes._t == t)
		return (false);
	_modes._t = t;
	return (true);
}

bool	Channel::enable_K(const bool k, const std::string& password)
{
	if (_modes._k == k)
		return (false);
	if (k == true && this->_password == password)
		return (false);
	if (k == false && this->_password != password)
		return (false);
	_modes._k = k;
	this->_password = password;
	return (true);
}

//renvoie change/non change
bool	Channel::enable_L(const std::size_t limit)
{
	if (limit == _userLimit)
		return (false);
	_userLimit = limit;
	return (true);
}

const std::string& Channel::getTopic() const
{
	return (this->_topic);
}

void Channel::setTopic(const std::string& topic, const Client& client)
{
	std::time_t currentTime = std::time(NULL);
	std::ostringstream oss;

	oss << currentTime;
	this->_date = oss.str();
	this->_topic = topic;
	this->_who_topic = client.getNick();
	// this->_date = std::to_string(std::time(NULL));
}

const std::string&	Channel::getDate() const
{
	return this->_date;
}

const std::string&	Channel::getPassword() const
{
	return (this->_password);
}
const std::vector<Client*>& Channel::getOperators() const
{
	return (this->_operators);
}

const std::vector<Client*>& Channel::getChanClients() const
{
	return (this->_chanClients);
}


void	Channel::sendToClients(const Client& sender, const std::string& msg)
{
	for (unsigned int i = 0; i < _chanClients.size(); i++)
		if (_chanClients[i] && _chanClients[i] != &sender)
			FormatIRC::sendPRIVMESS(_chanClients[i]->getFd(), sender.getNick(), this->_name, msg);
}

const std::string Channel::getNickList()
{
	std::string list;

	std::vector<Client*>::iterator itf;

	for (std::vector<Client*>::iterator it = _chanClients.begin(); it < _chanClients.end(); it ++)
	{
		itf = std::find(_operators.begin(), _operators.end(), (*it));
		if (itf != _operators.end())
			list += "@" + (*it)->getNick() + " ";
		else
			list += (*it)->getNick() + " ";
	}
	return (list);
}

const std::string Channel::getTopicInfo() const
{
	const std::string format = this->_who_topic + ' ' + this->_date;
	return (format);
}

bool	Channel::IsAnOp(const std::string nickname)
{
	for (std::vector<Client*>::iterator it = _operators.begin(); it != _operators.end(); it++)
	{
		if ((*it)->getNick() == nickname)
			return true;
	}
	return false;
}

bool	Channel::IsInChan(const std::string nickname)
{
	for (std::vector<Client*>::iterator it = _chanClients.begin(); it != _chanClients.end(); it++)
	{
		if ((*it)->getNick() == nickname)
			return true;
	}
	return false;
}



void	Channel::addClient(Client *client)
{
	if (this->_userLimit != 0 && _chanClients.size() >= this->_userLimit)
	{
		FormatIRC::sendCodeMsg(*client, "471", this->_name,  "Cannot join channel (+l)");
		throw (std::out_of_range("channel is full"));
	}
	if (_operators.size() == 0)
		addOperator(client);
	_chanClients.push_back(client);	
	std::cout<<  BG_GREEN << "client " << client->getNick() << " added to channel " << this->_name << " as Simple user" << RESET << std::endl;
}
void	Channel::addOperator(Client *client)
{
	_operators.push_back(client);
	std::cout << BG_BLUE << "client " << client->getNick() << " added to channel " << this->_name << " as Operator" << RESET << std::endl;
}

void	Channel::removeClient(const Client *client) //bot override it
{
	std::vector<Client*>::iterator	it;

	it = std::find(_chanClients.begin(), _chanClients.end(), client);
	if (it != _chanClients.end())
	_chanClients.erase(it);
}

void	Channel::removeClient(const std::string& nick)
{
	for (std::vector<Client*>::iterator	it = _chanClients.begin(); it < _chanClients.end(); it++)
	{
		if ((*it)->getNick() == nick)
		{
			_chanClients.erase(it);
			break;
		}
	}
}

void	Channel::removeOP(const Client& client)
{
	for (std::vector<Client*>::iterator	it = _operators.begin(); it < _operators.end(); it++)
	{
		if ((*it)->getNick() == client.getNick())
		{
			_operators.erase(it);
			break;
		}
	}
}