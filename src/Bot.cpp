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
	phase = Connect;
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

void	Bot::addClient(Client *client)
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

void	Bot::help(Client* client)
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

void	Bot::joke(Client* client)
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