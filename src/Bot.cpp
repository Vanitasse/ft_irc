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
	LG_createEmptyGame();
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
	}
	return *this;
}

void	Bot::addClient(Client *client)
{
	if (this->_userLimit != 0 && _chanClients.size() >= this->_userLimit)
	{
		FormatIRC::sendCodeMsg(*client, "471", this->_name,  "Cannot join channel (+l)");
		throw (std::out_of_range("channel is full"));
	}
	_chanClients.push_back(client);
	_chanClients.push_back(client);
	FormatIRC::sendNOTICE(client->getFd(), "#bot", "Bot rules here");
}

void	Bot::LG_createEmptyGame()
{
	t_LoupGarouGameID	game;

	game.game_ID = _LG_Games.size();
	game.phase = Connect;
	_LG_Games[_LG_Games.size()] = game;
}

void	Bot::LG_register(Client *client)
{
	t_LoupGarouPlayerID NewPlayerID;


	NewPlayerID.game_ID = _LG_Games.size();
	NewPlayerID.isDead = false;
	NewPlayerID.isPlaying = true;
	NewPlayerID.speech = 0;
	NewPlayerID.role = "none";
	client->setLG(NewPlayerID);
	_LG_Games[_LG_Games.size()].players.push_back(client);
	if (_LG_Games[_LG_Games.size()].players.size() == 5)
		LG_createEmptyGame();
}

// void	Bot::LG_launch
void	Bot::LG_play(Client *client)
{
	t_LoupGarouGameID*		game;


	if (client->getLG().isPlaying == false)
		LG_register(client);
	else
	{
		game = &_LG_Games[client->getLG().game_ID];
		switch (game->phase)
		{
			case Connect:
				FormatIRC::sendNOTICE(client->getFd(), "#bot", "We need more players before launching the game, please wait");
				break;
			case Roles:
				;
			case Election:
				;
			case Nuit:
				;
			case PetiteFille:
				;
			case Sorciere:
				;
			case Jour:
				;
		}
	}
}

