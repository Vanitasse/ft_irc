#pragma once

# include "Channel.hpp"

enum	e_LGphase
{
	Day,
	Night,
	Witch,
	
};
class	Bot : public Channel
{
	private:
	public:
		Bot();
		~Bot();
		Bot(const Bot& toCpy);
		Bot&	operator=(const Bot& toCpy);
		void	addClient(Client *client);
};