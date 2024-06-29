#pragma once

# include "Channel.hpp"

class	Bot : public Channel
{
	private:
		std::vector<Client*>	players;
		Client*					lastDead;
		e_LoupGarouPhases		phase;
		int						lifePotion;
		int						deathPotion;
		int						bullet;
		void	LG_register(Client *client);
		void	LG_story();
		bool	LG_speech(Client *client, const std::string& msg);
		bool	LG_vote(Client *client, const std::string& msg, const std::string& role);
		void	LG_newPhase(int speech, const std::string& role_vote);
		void	LG_infos(Client *client);
		void	LG_roles();
		void	LG_sendToPlayers(const std::string& notice, const std::string& role);
		Client*	LG_getMoreVoted();
		Client*	LG_findClientByRole(const std::string& role);


		void	help(Client* client);
		void	joke(Client* client);
		void	LG_play(Client *client, const std::string& msg);
		std::map<std::string, void (Bot::*)(Client*)> fmap;
	public:
		Bot();
		~Bot();
		Bot(const Bot& toCpy);
		Bot&	operator=(const Bot& toCpy);
		void	addClient(Client *client); //override
		void	parseMsg(Client* client, const std::string& msg);
};
