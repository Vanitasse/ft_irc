#include "../Channel.hpp"

Channel::Channel()
{
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
		this->_name = rhs._name;
		this->_operators = rhs._operators;
		this->_chanClients = rhs._chanClients;
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

const std::vector<Client*>& Channel::getOperators() const
{
	return (this->_operators);
}

const std::vector<Client*>& Channel::getChanClients() const
{
	return (this->_chanClients);
}

void	Channel::addClient(Client& client)
{
	if (_operators.size() == 0)
	{
		_operators.push_back(&client);
		std::cout << BG_BLUE << "client " << client.getNick() << " added to channel " << this->_name << " as Operator" << RESET << std::endl;
	}
	else
	{
		_chanClients.push_back(&client);
		std::cout<<  BG_GREEN << "client " << client.getNick() << " added to channel " << this->_name << " as Simple user" << RESET << std::endl;
	}
}

void	Channel::sendToClients(const Client& sender, const std::string& msg)
{
	for (auto& client : _operators)
		if (client->getFd() != sender.getFd())
			FormatIRC::SendPRIVMESS(client->getFd(), sender.getNick(), this->_name, msg);
	for (auto& client : _chanClients)
		if (client->getFd() != sender.getFd())
			FormatIRC::SendPRIVMESS(client->getFd(), sender.getNick(), this->_name, msg);
}
