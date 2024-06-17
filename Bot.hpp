#pragma once

# include "Channel.hpp"


class	Bot : public Channel
{
	private:
		int										_LG_launched_Games;
		std::map<int, t_LoupGarouGameID>		_LG_Games;
	public:
		Bot();
		~Bot();
		Bot(const Bot& toCpy);
		Bot&	operator=(const Bot& toCpy);

		void	addClient(Client *client); //override

		void	LG_createEmptyGame();
		void	LG_register(Client *client);
		void	LG_launch();
		void	LG_play(Client *client);
		void	parseMsg(const Client* client, const std::string& msg);

		void	sender(int fd, const std::string& format);
		void	help(const Client* client);
		void	joke(const Client* client);



};

typedef void (Bot::*t_ptr) (const Client* client);
