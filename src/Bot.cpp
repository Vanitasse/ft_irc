#include "../Bot.hpp"

Bot::Bot()
{
	std::cout << "Bot Created" << std::endl;
	std::time_t currentTime = std::time(NULL);
	std::ostringstream oss;
	oss << currentTime;
	_date = oss.str();

	// _date = std::to_string(std::time(NULL));
	_userLimit = 10;
	_name = "#bot";
	_modes._i = false;
	_modes._t = false;
	_modes._k = false;
	_modes._l = false;
	fmap["!help"] = &Bot::help;
	fmap["!joke"] = &Bot::joke;
	fmap["!lg"] = &Bot::LG_register;
}

Bot::~Bot() {}

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
		this->fmap = rhs.fmap;
	}
	return *this;
}

void	Bot::addClient(Client *client) //override
{
	if (this->_userLimit != 0 && _chanClients.size() >= this->_userLimit)
	{
		FormatIRC::sendCodeMsg(*client, "471", this->_name,  "Cannot join channel (+l)");
		throw (std::out_of_range("channel is full"));
	}
	_chanClients.push_back(client);
	FormatIRC::sendNOTICE(client->getFd(), "#bot", bot_rules);
	help(client);
}


void	Bot::parseMsg(Client* client, const std::string& msg)
{
	std::map<std::string, void (Bot::*)(Client*)>::iterator it;
		if (msg[0] == '!')
		{
			it = fmap.find(msg);
			if (it != fmap.end())
				(this->*(it->second))(client);
		}
		else if (client->getLG().isPlaying)
			LG_play(client, msg);
		else
			FormatIRC::sendNOTICE(client->getFd(), "#bot", "Bot channel: you can't chat in this channel, type !help to use bot");
}

void	Bot::removeFromGame(Client *client)
{
	std::vector<Client*>::iterator	it;

	it = std::find(games[client->getLG().ID].players.begin(), games[client->getLG().ID].players.end(), client);
	if (it != games[client->getLG().ID].players.end())
		games[client->getLG().ID].players.erase(it);
}