#include "../Channel.hpp"
#include <sys/time.h>

Channel::Channel()
{
	_date = std::to_string(std::time(NULL));
	_modes._i = false;
	_modes._t = false;
	_modes._k = false;
	_modes._o = false;
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
		this->_name = rhs.getName();
		this->_topic = rhs.getTopic();
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
void	Channel::setI(const bool i)
{
	_modes._i = i;
}
void	Channel::setT(const bool t)
{
	_modes._t = t;
}
void	Channel::setK(const bool k)
{
	_modes._k = k;
}
void	Channel::setO(const bool o)
{
	_modes._o = o;
}
void	Channel::setL(const bool l)
{
	_modes._l = l;
}

const std::string& Channel::getTopic() const
{
	return (this->_topic);
}

void Channel::setTopic(const std::string& topic, const Client& client)
{
	this->_topic = topic;
	this->_who_topic = client.getNick();
	this->_date = std::to_string(std::time(NULL));
}

// const std::vector<Client*>& Channel::getOperators() const
// {
// 	return (this->_operators);
// }

// const std::vector<Client*>& Channel::getChanClients() const
// {
// 	return (this->_chanClients);
// }


void	Channel::sendToClients(const Client& sender, const std::string& msg)
{
	for (unsigned int i = 0; i < _operators.size(); i++)
		if (_operators[i] && _operators[i] != &sender)
			FormatIRC::sendPRIVMESS(_operators[i]->getFd(), sender.getNick(), this->_name, msg);
	for (unsigned int i = 0; i < _chanClients.size(); i++)
		if (_chanClients[i] && _chanClients[i] != &sender)
			FormatIRC::sendPRIVMESS(_chanClients[i]->getFd(), sender.getNick(), this->_name, msg);
}

const std::string Channel::getNickList()
{
	std::string list;

	for (auto& client : _chanClients)
		list += client->getNick() + " ";
	for (auto& client : _operators)
		list += "@" + client->getNick() + " ";
	return (list);
}

const std::string Channel::getTopicInfo() const
{
	const std::string format = this->_who_topic + ' ' + this->_date;
	return (format);
}

bool	Channel::listOPs(const std::string nickname)
{
	for (std::vector<Client*>::iterator it = _operators.begin(); it < _operators.end(); it++)
	{
		if ((*it)->getNick() == nickname)
			return true;
	}
	return false;
}

void	Channel::addClient(Client *client)
{
	if (_operators.size() == 0)
	{
		_operators.push_back(client);
		std::cout << BG_BLUE << "client " << client->getNick() << " added to channel " << this->_name << " as Operator" << RESET << std::endl;
	}
	else
	{
		_chanClients.push_back(client);
		std::cout<<  BG_GREEN << "client " << client->getNick() << " added to channel " << this->_name << " as Simple user" << RESET << std::endl;
	}
}

void	Channel::removeClient(const Client *client)
{
	std::vector<Client*>::iterator	it;

	it = std::find(_operators.begin(), _operators.end(), client);
	if (it != _operators.end())
		_operators.erase(it);
	else
	{
		it = std::find(_chanClients.begin(), _chanClients.end(), client);
		if (it != _chanClients.end())
		_chanClients.erase(it);
	}
}
