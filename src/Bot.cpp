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
	FormatIRC::sendNOTICE(client->getFd(), "#bot", bot_rules);
}

void	Bot::sender(int fd, const std::string& format)
{
	const std::string res(format + "\r\n");

	send(fd, res.c_str(), res.length(), 0);
	std::cout << CYAN << "[Server send]" << res << RESET << std::endl;
}

void	Bot::help(const Client* client)
{
	std::string format = "---Here is the list of all the available commands---";
	FormatIRC::sendNOTICE(client->getFd(), "#bot", format);
	format = "!joke : i will tell you one of my best jokes";
	FormatIRC::sendNOTICE(client->getFd(), "#bot", format);
	format = "!LG : launch a LOUP-GAROU game" ;
	FormatIRC::sendNOTICE(client->getFd(), "#bot", format);
	format = "!help : show this menu";
	FormatIRC::sendNOTICE(client->getFd(), "#bot", format);

}

void	Bot::joke(const Client* client)
{
	std::srand(std::time(0));
	int random_index = std::rand() % 30;
		std::string jokes[] = {
		"Why did the robot go to school? To improve its byte-sized education!",
		"What did the robot say to the vending machine? 'I know you're a fellow machine, but I'm not buying it.'",
		"Why was the robot so bad at soccer? It kept kicking up sparks!",
		"How do robots pay for things? With cache, of course!",
		"Why did the robot apply for a job? It wanted to make its own decisions, not just follow protocols.",
		"What’s a robot’s favorite music genre? Heavy metal!",
		"Why did the robot get a promotion? It had outstanding bits of work.",
		"How do robots flirt? They give each other lots of RAM-antic compliments.",
		"Why did the robot join a band? It had a great set of drums!",
		"What do you call a robot who likes to tell jokes? A bit funny!",
		"Why did the robot go on a diet? It had too many bytes.",
		"How do robots exercise? They do circuits.",
		"Why do robots make terrible secret agents? They always give binary answers.",
		"What did the robot say to the human who stepped on its foot? 'I feel a bit compressed.'",
		"Why did the robot start a podcast? To byte into new conversations!",
		"Why did the robot get lost? It took the wrong software turn.",
		"How do robots write poetry? In algorithms and rhymes.",
		"Why did the robot get a driver's license? To navigate the information superhighway.",
		"What do you call a robot that always takes the blame? A scapebot.",
		"Why did the robot visit the therapist? It had too many memory issues.",
		"How do robots communicate with each other? They use their intel-ligence.",
		"Why did the robot bring a ladder to work? To reach the cloud!",
		"What’s a robot’s favorite movie? 'A.I.: Artificial Intelligence.'",
		"Why did the robot go on a diet? It was carrying too many chips.",
		"How do you make a robot laugh? You tickle its funny byte.",
		"Why was the robot so calm? It had excellent control over its emotions.",
		"What did the robot say after a date? 'I had an electrifying time!'",
		"Why did the robot become a chef? It loved to cook up new programs.",
		"How do robots celebrate? They throw a bit party.",
		"What did the robot do at the art gallery? It critiqued the brushwork with its pixel precision."
	};
	std::string format = jokes[random_index];
	FormatIRC::sendNOTICE(client->getFd(), "#bot", format);
}


void	Bot::parseMsg(const Client* client, const std::string& msg)
{
	std::cout << "OUI ON Y EST" << std::endl;
	t_ptr ptr[] = {&Bot::help, &Bot::joke};
	int i = 0;
	std::string inputs[] = {"!help\r\n", "!joke\r\n"};
	std::cout << "MSG = " << msg << std::endl;
	while (i < 1 && inputs[i] != msg)
		i++;
	(this->*ptr[i])(client);
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

