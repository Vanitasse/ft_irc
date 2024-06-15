#pragma once

# include "Channel.hpp"

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