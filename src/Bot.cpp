#include "../Bot.hpp"

Bot::Bot()
{
	_isConnected = true;
	_fd = -1;
	_passIsSet = true;
	_nickIsSet = true;
	_nick = "Bot";

}

Bot::Bot(const Bot& toCpy)
{
	*this = toCpy;
}

Bot::~Bot()
{

}

Bot&	Bot::operator=(const Bot& toCpy)
{
	if (this != &toCpy)
	{
		_isConnected = toCpy._isConnected;
		_passIsSet = toCpy._passIsSet;
		_nickIsSet = toCpy._nickIsSet;
		_nick = toCpy._nick;
		_server = toCpy._server;
		_fd = toCpy._fd;
	}

	return (*this);
}

Bot::Bot(Server& server)
{
	_isConnected = true;
	_passIsSet = true;
	_nickIsSet = true;
	_nick = "Bot";
	_server = &server;
	_fd = -1;

}
