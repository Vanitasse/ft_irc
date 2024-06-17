#include "../Bot.hpp"

Bot::Bot()
{
	std::cout << "Bot Created" << std::endl;
	_date = std::to_string(std::time(NULL));
	_userLimit = 10;
	_name = "#bot";
	_modes._i = false;
	_modes._t = false;
	_modes._k = false;
	_modes._l = false;
}
Bot::~Bot()
{
}
Bot::Bot(const Bot& toCpy)
{
	*this = toCpy;
}
Bot&	Bot::operator=(const Bot& rhs)
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

void	Bot::addClient(Client *client)
{
	std::cout << "lol" << std::endl;
	if (this->_userLimit != 0 && _chanClients.size() >= this->_userLimit)
	{
		FormatIRC::sendCodeMsg(*client, "471", this->_name,  "Cannot join channel (+l)");
		throw (std::out_of_range("channel is full"));
	}
	_chanClients.push_back(client);
	FormatIRC::sendNOTICE(client->getFd(), "#bot", "Bot rules here");
}