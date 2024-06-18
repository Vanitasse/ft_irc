#pragma once

# include "Channel.hpp"

class	Bot : public Channel
{
	private:
		std::vector<Client*>	players;
		e_LoupGarouPhases		phase;
		std::map<std::string, void (Bot::*)(Client*)> fmap;
		void	LG_register(Client *client);
		void	LG_Story_election();
		void	LG_Story_vote();
		void	LG_Story_night();
		void	LG_Story_day();


		void	LG_play(Client *client, const std::string& msg);

		bool	LG_speech(Client *client, const std::string& msg);
		bool	LG_vote(Client *client, const std::string& msg, const std::string& role);
		void	LG_newPhase(int speech);
		Client*	LG_getMoreVoted();

		void	help(Client* client);
		void	joke(Client* client);
	public:
		Bot();
		~Bot();
		Bot(const Bot& toCpy);
		Bot&	operator=(const Bot& toCpy);
		void	addClient(Client *client); //override
		void	parseMsg(Client* client, const std::string& msg);
};

typedef void (Bot::*t_ptr) (Client* client);
