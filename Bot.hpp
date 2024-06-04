#pragma once

# include "Client.hpp"

class	Bot : public Client
{
	private:
	public:
		Bot();
		~Bot();
		Bot(const Bot& toCpy);
		Bot&	operator=(const Bot& toCpy);
		Bot(Server& server);
};