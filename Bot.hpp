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
};