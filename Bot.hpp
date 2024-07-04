#pragma once

# include "Channel.hpp"

typedef struct	s_lg_game
{
	std::vector<Client*>	players;
	Client*					lastDead;
	e_LoupGarouPhases		phase;
	int						lifePotion;
	int						deathPotion;
}				t_lg_game;

class	Bot : public Channel
{
	private:
		std::map<int, t_lg_game>	games;
		void	LG_register(Client *client);
		void	LG_story(Client* client);
		bool	LG_speech(Client *client, const std::string& msg);
		bool	LG_vote(Client *client, const std::string& msg, const std::string& role);
		bool	LG_newPhase(std::vector<Client*>	players, int speech, const std::string& role_vote);
		void	LG_infos(Client *client);
		void	LG_roles(std::vector<Client*> players);
		void	LG_sendToPlayers(std::vector<Client*> players, const std::string& notice, const std::string& role);
		Client*	LG_getMoreVoted(std::vector<Client*> players);
		Client*	LG_findClientByRole(std::vector<Client*>	players, const std::string& role);


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
		void	removeFromGame(Client* client);
};
