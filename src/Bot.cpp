#include "../Bot.hpp"

Bot::Bot()
{
	_date = std::to_string(std::time(NULL));
	_userLimit = 0;
	_modes._i = false;
	_modes._t = false;
	_modes._k = false;
	_modes._l = false;
	_topic = "Play with Bot";
	_name = "#Bot";
}

Bot::Bot(const Bot& toCpy)
{
	*this = toCpy;
}

Bot::~Bot()
{

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
	_chanClients.push_back(client);	
	std::cout<<  BG_GREEN << "client " << client->getNick() << " added to channel " << this->_name << " as Simple user" << RESET << std::endl;
}