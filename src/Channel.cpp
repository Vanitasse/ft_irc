#include "../Channel.hpp"
#include <sys/time.h>

Channel::Channel()
{
	_date = std::to_string(std::time(NULL));
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

const std::string& Channel::getTopic() const
{
	return (this->_topic);
}

void Channel::setTopic(const std::string& topic, const Client& client)
{
	this->_topic = topic;
	this->_who_topic = client.getNick();
}

// const std::vector<Client*>& Channel::getOperators() const
// {
// 	return (this->_operators);
// }

// const std::vector<Client*>& Channel::getChanClients() const
// {
// 	return (this->_chanClients);
// }

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

void	Channel::channelClearClient(const Client* client)
{
	std::vector<Client*>::iterator it;

	it = std::find(_operators.begin(), _operators.end(), client);
	if (it != _operators.end())
		_operators.erase(it);
	it = std::find(_chanClients.begin(), _chanClients.end(), client);
	if (it != _chanClients.end())
		_chanClients.erase(it);
}

