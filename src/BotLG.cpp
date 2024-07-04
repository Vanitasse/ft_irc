#include "../Bot.hpp"
#include <fstream>
#include <string>

void	Bot::LG_register(Client *client)
{
	t_LoupGarouPlayerID NewPlayerID;
	std::size_t			nb_players(3);
	std::ostringstream	oss;
	std::ifstream		infile;
	std::string			line;
	static int			launched_games = 0;

	NewPlayerID.ID = launched_games;
	NewPlayerID.isDead = false;
	NewPlayerID.isPlaying = true;
	NewPlayerID.voted = false;
	NewPlayerID.maire = false;
	NewPlayerID.votes = 0;
	NewPlayerID.speech = 2;
	client->setLG(NewPlayerID);

	games[launched_games].players.push_back(client);
	games[launched_games].phase = Connect;
	games[launched_games].lifePotion = 1;
	games[launched_games].deathPotion = 1;
	games[launched_games].lastDead = NULL;

	infile.open("logo.txt");
	if (infile.is_open())
	{
		while (std::getline(infile, line))
			FormatIRC::sendNOTICE(client->getFd(), "#bot", RED + line + RESET);
		infile.close();
	}
	FormatIRC::sendNOTICE(client->getFd(), "#bot", "Registered to Loup-Garou game");
	FormatIRC::sendNOTICE(client->getFd(), "#bot", "");
	FormatIRC::sendNOTICE(client->getFd(), "#bot",  BOLD ITALIC GREEN StoryIntro RESET);
	if (nb_players - games[launched_games].players.size())
	{
		oss << nb_players - games[launched_games].players.size();
		LG_sendToPlayers(games[launched_games].players, "Waiting for " + oss.str() + " more players", "all");
	}
	if (games[launched_games].players.size() == nb_players)
	{
		games[launched_games].phase = ElectionSpeechs;
		LG_roles(games[launched_games].players);
		LG_story(client);
		launched_games++;
	}
}

void	Bot::LG_roles(std::vector<Client*>	players)
{
	int			random;
	std::size_t	sorciere(1);
	std::size_t	voyante(1);
	std::size_t	loups_garous(0);
	std::size_t	villageois(0);

	for (std::vector<Client*>::iterator it = players.begin(); it != players.end(); it++)
	{
		if (rand() % 2 == 0)
		{
			(*it)->getLG().role = "Loup-Garou";
			loups_garous++;
		}
		else
		{
			random = rand() % 4;
			if (random == 0 && sorciere == 1)
			{
				(*it)->getLG().role = "Sorciere";
				sorciere = 0;
			}
			else if (random == 3 && voyante == 1)
			{
				(*it)->getLG().role = "Voyante";
				voyante = 0;
			}
			else
				(*it)->getLG().role = "Villageois";
			villageois++;
		}
	}
	if (!loups_garous || !villageois)
		LG_roles(players);
}

void	Bot::LG_infos(Client *client)
{
	std::ostringstream oss;
	std::string format;

	format = std::string("Vous êtes un(e) ") + BLUE + client->getLG().role + RESET;
	FormatIRC::sendNOTICE(client->getFd(), "#bot", format);
	oss << client->getLG().speech;
	switch (games[client->getLG().ID].phase)
	{
		case Connect:
			break;
		case ElectionSpeechs:
			format = "Vous avez " GREEN + oss.str() + RESET + " possibilié(s) de parler, tapez " RED "stop" RESET " si vous ne voulez plus parler";
			break;
		case ElectionVotes:
			format = "Vous devez voter pour cette phase en ecrivant le nom d'un joueur";
			break;
		case Voyante:
			if (client->getLG().role == "Voyante")
				format = "Vous devez voter pour cette phase en ecrivant le nom d'un joueur";
			else
				format = "Vous dormez";
			break;
		case LoupsGarous:
			if (client->getLG().role == "Loup-Garou")
				format = "Vous devez voter pour cette phase en ecrivant le nom d'un joueur";
			else
				format = "Vous dormez";
			break;
		case Sorciere:
			if (client->getLG().role == "Sorciere")
				format = "Vous devez voter pour cette phase en ecrivant le nom d'un joueur";
			else
				format = "Vous dormez";
			break;
		case JourSpeech:
			format = "Vous avez " GREEN + oss.str() + RESET + " possibilié(s) de parler";
			break;
		case JourVotes:
			format = "Vous devez voter pour cette phase en ecrivant le nom d'un joueur";
			break;
	}
	FormatIRC::sendNOTICE(client->getFd(), "#bot", format);
}

bool	Bot::LG_speech(Client *client, const std::string& msg)
{
	int	total_speechs(0);

	if (client->getLG().speech == 0)
		return (FormatIRC::sendNOTICE(client->getFd(), "#bot", "Vous avez déjà assez parlé..."), false);
	client->getLG().speech--;
	for (std::vector<Client*>::iterator it = games[client->getLG().ID].players.begin(); it != games[client->getLG().ID].players.end(); it++)
	{
		if ((*it)->getFd() != client->getFd())
			FormatIRC::sendPRIVMESS((*it)->getFd(), client->getNick(), this->_name, msg);
		total_speechs += (*it)->getLG().speech;
	}
	if (!total_speechs)
		return (true);
	return (false);
}

bool	Bot::LG_vote(Client *client, const std::string& msg, const std::string& role)
{
	int		vote_remaining(1);
	int		vote_power(1);

	if (client->getLG().role != role && role != "all")
		return (FormatIRC::sendNOTICE(client->getFd(), "#bot", "Ce n'est pas votre tour") ,false);
	if (client->getLG().voted == true)
		return (FormatIRC::sendNOTICE(client->getFd(), "#bot", "Vous avez deja voté") ,false);
	if (client->getNick() == msg && games[client->getLG().ID].phase != Sorciere && games[client->getLG().ID].phase != ElectionVotes)
		return (FormatIRC::sendNOTICE(client->getFd(), "#bot", "Vous ne pouvez pas voter pour vous même") ,false);
	if (games[client->getLG().ID].phase == JourVotes && client->getLG().maire == true)
		vote_power = 2;
	if (client->getLG().role == role || role == "all")
	{
		for (std::vector<Client*>::iterator it = games[client->getLG().ID].players.begin(); it != games[client->getLG().ID].players.end(); it++)
		{
			if (msg == (*it)->getNick())
			{
				if ((*it)->getLG().isDead)
					return (FormatIRC::sendNOTICE(client->getFd(), "#bot", "Ce joueur est mort") ,false);
				client->getLG().voted = true;
				(*it)->getLG().votes += vote_power;
			}
			if ((*it)->getLG().voted == false && *it != client)
				vote_remaining++;
		}
	}
	if (client->getLG().voted)
	{
		FormatIRC::sendNOTICE(client->getFd(), "#bot", "Vous avez voté pour " GREEN + msg + RESET);
		vote_remaining--;
	}
	else
		FormatIRC::sendNOTICE(client->getFd(), "#bot", "Le joueur " GREEN + msg + RESET " n'existe pas");
	if (vote_remaining)
		return (false);
	return (true);
}

void	Bot::LG_sendToPlayers(std::vector<Client*>	players, const std::string& notice, const std::string& role)
{
		for (std::vector<Client*>::iterator it = players.begin(); it != players.end(); it++)
		{
			if (role == "all" || (*it)->getLG().role == role)
				FormatIRC::sendNOTICE((*it)->getFd(), "#bot", notice);
		}
}

Client*	Bot::LG_findClientByRole(std::vector<Client*> players, const std::string& role)
{
	for (std::vector<Client*>::iterator it = players.begin(); it != players.end(); it++)
	{
		if ((*it)->getLG().role == role)
			return (*it);
	}
	return (NULL);
}

Client*	Bot::LG_getMoreVoted(std::vector<Client*> players)
{
	int	vote = 0;
	Client	*more_voted;

	for (std::vector<Client*>::iterator it = players.begin(); it != players.end(); it++)
	{
		if ((*it)->getLG().votes > vote)
		{
			vote = (*it)->getLG().votes;
			more_voted = *it;
		}
		else if ((*it)->getLG().votes == vote)
			more_voted = NULL;
	}
	return (more_voted);
}

void	Bot::LG_play(Client *client, const std::string& msg)
{
	Client *tmp;
	std::vector<Client*>::iterator it;

	if (client->getLG().isDead)
		return (FormatIRC::sendNOTICE(client->getFd(), "#bot", "Vous êtes mort, vous ne pouvez plus interagir avec les autres joueurs"));
	if (msg == "stop")
		client->getLG().speech = 1;
	switch (games[client->getLG().ID].phase)
	{
		case Connect:
			FormatIRC::sendNOTICE(client->getFd(), "#bot", "We need more players before launching the game, please wait");
			break;
		case ElectionSpeechs:
			if (LG_speech(client, msg))
			{
				games[client->getLG().ID].phase = ElectionVotes;
				if (!LG_newPhase(games[client->getLG().ID].players, 0, "all"))
					return ;
				LG_story(client);
			}
			break;
		case ElectionVotes:
			if (LG_vote(client, msg, "all"))
			{
				tmp = LG_getMoreVoted(games[client->getLG().ID].players);
				if (tmp == NULL)
				{
					LG_sendToPlayers(games[client->getLG().ID].players, BG_BRIGHT_RED BOLD "le vote s'est solde par une egalite, votez de nouveau" RESET, "all");
					if (!LG_newPhase(games[client->getLG().ID].players, 0, "all"))
						return ;
				}
				else
				{
					LG_sendToPlayers(games[client->getLG().ID].players, BG_BRIGHT_RED BOLD + tmp->getNick() + RESET + BG_BRIGHT_RED BOLD " a ete elu maire du village" RESET, "all");
					tmp->getLG().maire = true;
					tmp = LG_findClientByRole(games[client->getLG().ID].players, "Voyante");
					if (tmp && !tmp->getLG().isDead)
					{
						games[client->getLG().ID].phase = Voyante;
						if (!LG_newPhase(games[client->getLG().ID].players, 0, "Voyante"))
							return ;
						LG_story(client);
					}
					else
					{
						games[client->getLG().ID].phase = LoupsGarous;
						if (!LG_newPhase(games[client->getLG().ID].players, 0, "Loup-Garou"))
							return ;
						LG_story(client);
					}
				}
			}
			break;
		case Voyante:
			if (LG_vote(client, msg, "Voyante"))
			{
				games[client->getLG().ID].phase = LoupsGarous;
				tmp = LG_getMoreVoted(games[client->getLG().ID].players);
				FormatIRC::sendNOTICE(client->getFd(), "#bot", BG_BRIGHT_RED BOLD "le role de ce joueur est " BLUE + tmp->getLG().role + RESET);
				if (!LG_newPhase(games[client->getLG().ID].players, 0, "Loup-Garou"))
					return ;
				LG_story(client);
			}
			break;
		case LoupsGarous:
			if (LG_vote(client, msg, "Loup-Garou"))
			{
				games[client->getLG().ID].lastDead = LG_getMoreVoted(games[client->getLG().ID].players);
				if (games[client->getLG().ID].lastDead == NULL)
				{
					LG_sendToPlayers(games[client->getLG().ID].players, BG_BRIGHT_RED BOLD "le vote s'est solde par une egalite, votez de nouveau" RESET, "Loup-Garou");
					if (!LG_newPhase(games[client->getLG().ID].players, 0, "Loup-Garou"))
						return ;
				}
				else
				{
					games[client->getLG().ID].lastDead->getLG().isDead = true;
					it = std::find(games[client->getLG().ID].players.begin(), games[client->getLG().ID].players.end(), games[client->getLG().ID].lastDead);
					games[client->getLG().ID].players.erase(it);
					tmp = LG_findClientByRole(games[client->getLG().ID].players, "Sorciere");
					if (tmp && !tmp->getLG().isDead && (games[client->getLG().ID].lifePotion || games[client->getLG().ID].deathPotion))
					{
						games[client->getLG().ID].phase = Sorciere;
						if (!LG_newPhase(games[client->getLG().ID].players, 0, "Sorciere"))
							return ;
						LG_story(client);
						FormatIRC::sendNOTICE(tmp->getFd(), "#bot", BG_BRIGHT_RED BOLD "le joueur " GREEN + games[client->getLG().ID].lastDead->getNick() + RESET + BG_BRIGHT_RED BOLD " est mort cette nuit, que comptes tu faire?" RESET);
					}
					else
					{
						games[client->getLG().ID].phase = JourSpeech;
						LG_sendToPlayers(games[client->getLG().ID].players, BG_BRIGHT_RED BOLD "le joueur " GREEN + games[client->getLG().ID].lastDead->getNick() + RESET + BG_BRIGHT_RED BOLD " est mort, son rôle etait " BLUE + games[client->getLG().ID].lastDead->getLG().role + RESET, "all");
						if (!LG_newPhase(games[client->getLG().ID].players, 3, "all"))
							return ;
						LG_story(client);
					}
				}
			}
			break;
		case Sorciere:
			if (LG_vote(client, msg, "Sorciere"))
			{
				tmp = LG_getMoreVoted(games[client->getLG().ID].players);
				if (tmp && (tmp->getNick() == games[client->getLG().ID].lastDead->getNick()))
				{
					games[client->getLG().ID].lastDead->getLG().isDead = false;
					games[client->getLG().ID].lastDead = NULL;
					tmp = NULL;
					games[client->getLG().ID].players.push_back(games[client->getLG().ID].lastDead);
				}
				else if (tmp && (tmp->getNick() != games[client->getLG().ID].lastDead->getNick()))
				{
					tmp->getLG().isDead = true;
					it = std::find(games[client->getLG().ID].players.begin(), games[client->getLG().ID].players.end(), tmp);
					games[client->getLG().ID].players.erase(it);
				}
				if (games[client->getLG().ID].lastDead == NULL && tmp == NULL)
					LG_sendToPlayers(games[client->getLG().ID].players, BG_BRIGHT_RED BOLD "Personne n'est mort cette nuit", "all");
				else if (games[client->getLG().ID].lastDead)
					LG_sendToPlayers(games[client->getLG().ID].players, BG_BRIGHT_RED BOLD "le joueur " GREEN + games[client->getLG().ID].lastDead->getNick() + " est mort cette nuit, son rôle etait " BLUE + games[client->getLG().ID].lastDead->getLG().role + RESET, "all");
				else if (tmp)
					LG_sendToPlayers(games[client->getLG().ID].players, BG_BRIGHT_RED BOLD "le joueur " GREEN + tmp->getNick() + " est mort cette nuit, son rôle etait " BLUE + tmp->getLG().role + RESET, "all");
				games[client->getLG().ID].phase = JourSpeech;
				if (!LG_newPhase(games[client->getLG().ID].players, 3, "all"))
					return ;
				LG_story(client);
			}
			break;
		case JourSpeech:
			if (LG_speech(client, msg))
			{
				games[client->getLG().ID].phase = JourVotes;
				if (!LG_newPhase(games[client->getLG().ID].players, 0, "all"))
					return ;
				LG_story(client);
			}
			break;
		case JourVotes:
			if (LG_vote(client, msg, "all"))
			{
				games[client->getLG().ID].lastDead = LG_getMoreVoted(games[client->getLG().ID].players);
				if (games[client->getLG().ID].lastDead == NULL)
				{
					LG_sendToPlayers(games[client->getLG().ID].players, BG_BRIGHT_RED BOLD "le vote s'est solde par une egalite, votez de nouveau", "all");
					if (!LG_newPhase(games[client->getLG().ID].players, 0, "all"))
						return ;
				}
				else
				{
					games[client->getLG().ID].lastDead->getLG().isDead = true;
					it = std::find(games[client->getLG().ID].players.begin(), games[client->getLG().ID].players.end(), games[client->getLG().ID].lastDead);
					games[client->getLG().ID].players.erase(it);
					LG_sendToPlayers(games[client->getLG().ID].players, BG_BRIGHT_RED BOLD "le joueur " GREEN + games[client->getLG().ID].lastDead->getNick() + RESET + BG_BRIGHT_RED BOLD " est pendu, son rôle etait " BLUE + games[client->getLG().ID].lastDead->getLG().role + RESET, "all");
					if (LG_findClientByRole(games[client->getLG().ID].players, "Voyante"))
					{
						games[client->getLG().ID].phase = Voyante;
						if (!LG_newPhase(games[client->getLG().ID].players, 0, "Voyante"))
							return ;
						LG_story(client);
					}
					else
					{
						games[client->getLG().ID].phase = LoupsGarous;
						if (!LG_newPhase(games[client->getLG().ID].players, 0, "Loup-Garou"))
							return ;
						LG_story(client);
					}
				}
			}
			break;
	}
}

void	Bot::LG_story(Client* client)
{
	for (std::vector<Client*>::iterator it = games[client->getLG().ID].players.begin(); it != games[client->getLG().ID].players.end(); it++)
	{
		for (int i = 0; i < 8; i++)
			FormatIRC::sendNOTICE((*it)->getFd(), "#bot", "");
		switch (games[client->getLG().ID].phase)
		{
			case Connect:
				break;
			case ElectionSpeechs:
				FormatIRC::sendNOTICE((*it)->getFd(), "#bot", GREEN election0 RESET);
				FormatIRC::sendNOTICE((*it)->getFd(), "#bot", GREEN election1 RESET);
				FormatIRC::sendNOTICE((*it)->getFd(), "#bot", GREEN election2 RESET);
				FormatIRC::sendNOTICE((*it)->getFd(), "#bot", GREEN election3 RESET);
				FormatIRC::sendNOTICE((*it)->getFd(), "#bot", GREEN election4 RESET);
				FormatIRC::sendNOTICE((*it)->getFd(), "#bot", GREEN election5 RESET);
				FormatIRC::sendNOTICE((*it)->getFd(), "#bot", ITALIC BOLD GREEN StoryElection RESET);
				if (!(*it)->getLG().isDead)
					FormatIRC::sendNOTICE((*it)->getFd(), "#bot", BLINK_SLOW BOLD YELLOW "VOTRE TOUR" RESET);
				break;
			case ElectionVotes:
				FormatIRC::sendNOTICE((*it)->getFd(), "#bot", CYAN voteMaire0 RESET);
				FormatIRC::sendNOTICE((*it)->getFd(), "#bot", CYAN voteMaire1 RESET);
				FormatIRC::sendNOTICE((*it)->getFd(), "#bot", CYAN voteMaire2 RESET);
				FormatIRC::sendNOTICE((*it)->getFd(), "#bot", CYAN voteMaire3 RESET);
				FormatIRC::sendNOTICE((*it)->getFd(), "#bot", CYAN voteMaire4 RESET);
				FormatIRC::sendNOTICE((*it)->getFd(), "#bot", CYAN voteMaire5 RESET);
				FormatIRC::sendNOTICE((*it)->getFd(), "#bot", CYAN voteMaire6 RESET);
				FormatIRC::sendNOTICE((*it)->getFd(), "#bot", CYAN voteMaire7 RESET);
				FormatIRC::sendNOTICE((*it)->getFd(), "#bot", ITALIC BOLD GREEN StoryVoteMaire RESET);
				if (!(*it)->getLG().isDead)
					FormatIRC::sendNOTICE((*it)->getFd(), "#bot", BLINK_SLOW BOLD YELLOW "VOTRE TOUR" RESET);
				break;
			case Voyante:
				FormatIRC::sendNOTICE((*it)->getFd(), "#bot", CYAN voyante0 RESET);
				FormatIRC::sendNOTICE((*it)->getFd(), "#bot", CYAN voyante1 RESET);
				FormatIRC::sendNOTICE((*it)->getFd(), "#bot", CYAN voyante2 RESET);
				FormatIRC::sendNOTICE((*it)->getFd(), "#bot", CYAN voyante3 RESET);
				FormatIRC::sendNOTICE((*it)->getFd(), "#bot", CYAN voyante4 RESET);
				FormatIRC::sendNOTICE((*it)->getFd(), "#bot", CYAN voyante5 RESET);
				FormatIRC::sendNOTICE((*it)->getFd(), "#bot", CYAN voyante6 RESET);
				FormatIRC::sendNOTICE((*it)->getFd(), "#bot", CYAN voyante7 RESET);
				FormatIRC::sendNOTICE((*it)->getFd(), "#bot", ITALIC BOLD GREEN StoryVoyante RESET);
				if (!(*it)->getLG().isDead && (*it)->getLG().role == "Voyante")
					FormatIRC::sendNOTICE((*it)->getFd(), "#bot", BLINK_SLOW BOLD YELLOW "VOTRE TOUR" RESET);
				break;
			case LoupsGarous:
				FormatIRC::sendNOTICE((*it)->getFd(), "#bot", RED loupsGarous0 RESET);
				FormatIRC::sendNOTICE((*it)->getFd(), "#bot", RED loupsGarous1 RESET);
				FormatIRC::sendNOTICE((*it)->getFd(), "#bot", RED loupsGarous2 RESET);
				FormatIRC::sendNOTICE((*it)->getFd(), "#bot", RED loupsGarous3 RESET);
				FormatIRC::sendNOTICE((*it)->getFd(), "#bot", RED loupsGarous4 RESET);
				FormatIRC::sendNOTICE((*it)->getFd(), "#bot", RED loupsGarous5 RESET);
				FormatIRC::sendNOTICE((*it)->getFd(), "#bot", RED loupsGarous6 RESET);
				FormatIRC::sendNOTICE((*it)->getFd(), "#bot", RED loupsGarous7 RESET);
				FormatIRC::sendNOTICE((*it)->getFd(), "#bot", ITALIC BOLD GREEN StoryLoupsGarous RESET);
				if (!(*it)->getLG().isDead && (*it)->getLG().role == "Loup-Garou")
					FormatIRC::sendNOTICE((*it)->getFd(), "#bot", BLINK_SLOW BOLD YELLOW "VOTRE TOUR" RESET);
				break;
			case Sorciere:
				FormatIRC::sendNOTICE((*it)->getFd(), "#bot", YELLOW sorciere0 RESET);
				FormatIRC::sendNOTICE((*it)->getFd(), "#bot", YELLOW sorciere1 RESET);
				FormatIRC::sendNOTICE((*it)->getFd(), "#bot", YELLOW sorciere2 RESET);
				FormatIRC::sendNOTICE((*it)->getFd(), "#bot", YELLOW sorciere3 RESET);
				FormatIRC::sendNOTICE((*it)->getFd(), "#bot", YELLOW sorciere4 RESET);
				FormatIRC::sendNOTICE((*it)->getFd(), "#bot", YELLOW sorciere5 RESET);
				FormatIRC::sendNOTICE((*it)->getFd(), "#bot", YELLOW sorciere6 RESET);
				FormatIRC::sendNOTICE((*it)->getFd(), "#bot", YELLOW sorciere7 RESET);
				if (games[client->getLG().ID].lifePotion && games[client->getLG().ID].deathPotion)
					FormatIRC::sendNOTICE((*it)->getFd(), "#bot", ITALIC BOLD GREEN StorySorciere0 RESET);
				else if (games[client->getLG().ID].lifePotion)
					FormatIRC::sendNOTICE((*it)->getFd(), "#bot", ITALIC BOLD GREEN StorySorciere1 RESET);
				else if (games[client->getLG().ID].deathPotion)
					FormatIRC::sendNOTICE((*it)->getFd(), "#bot", ITALIC BOLD GREEN StorySorciere2 RESET);
				if (!(*it)->getLG().isDead && (*it)->getLG().role == "Sorciere")
					FormatIRC::sendNOTICE((*it)->getFd(), "#bot", BLINK_SLOW BOLD YELLOW "VOTRE TOUR" RESET);
				break;
			case JourSpeech:
				FormatIRC::sendNOTICE((*it)->getFd(), "#bot", GREEN villageois0 RESET);
				FormatIRC::sendNOTICE((*it)->getFd(), "#bot", GREEN villageois1 RESET);
				FormatIRC::sendNOTICE((*it)->getFd(), "#bot", GREEN villageois2 RESET);
				FormatIRC::sendNOTICE((*it)->getFd(), "#bot", GREEN villageois3 RESET);
				FormatIRC::sendNOTICE((*it)->getFd(), "#bot", GREEN villageois4 RESET);
				FormatIRC::sendNOTICE((*it)->getFd(), "#bot", GREEN villageois5 RESET);
				FormatIRC::sendNOTICE((*it)->getFd(), "#bot", GREEN villageois6 RESET);
				FormatIRC::sendNOTICE((*it)->getFd(), "#bot", GREEN villageois7 RESET);
				FormatIRC::sendNOTICE((*it)->getFd(), "#bot", ITALIC BOLD GREEN StoryDay RESET);
				if (!(*it)->getLG().isDead)
					FormatIRC::sendNOTICE((*it)->getFd(), "#bot", BLINK_SLOW BOLD YELLOW "VOTRE TOUR" RESET);
				break;
			case JourVotes:
				FormatIRC::sendNOTICE((*it)->getFd(), "#bot", GREEN coupable0 RESET);
				FormatIRC::sendNOTICE((*it)->getFd(), "#bot", GREEN coupable1 RESET);
				FormatIRC::sendNOTICE((*it)->getFd(), "#bot", GREEN coupable2 RESET);
				FormatIRC::sendNOTICE((*it)->getFd(), "#bot", GREEN coupable3 RESET);
				FormatIRC::sendNOTICE((*it)->getFd(), "#bot", GREEN coupable4 RESET);
				FormatIRC::sendNOTICE((*it)->getFd(), "#bot", GREEN coupable5 RESET);
				FormatIRC::sendNOTICE((*it)->getFd(), "#bot", GREEN coupable6 RESET);
				FormatIRC::sendNOTICE((*it)->getFd(), "#bot", GREEN coupable7 RESET);
				FormatIRC::sendNOTICE((*it)->getFd(), "#bot", ITALIC BOLD GREEN StoryDayVote RESET);
				if (!(*it)->getLG().isDead)
					FormatIRC::sendNOTICE((*it)->getFd(), "#bot", BLINK_SLOW BOLD YELLOW "VOTRE TOUR" RESET);
				break;
		}
		LG_infos(*it);
		for (int i = 0; i < 8; i++)
			FormatIRC::sendNOTICE((*it)->getFd(), "#bot", "");
	}
}

bool	Bot::LG_newPhase(std::vector<Client*> players, int speech, const std::string& role_vote)
{
	int	goods;
	int	bads;

	goods = 0;
	bads = 0;
	for (std::vector<Client*>::iterator it = players.begin(); it != players.end(); it++)
	{
		if (!(*it)->getLG().isDead && (*it)->getLG().role == "Loup-Garou")
			bads++;
		else if (!(*it)->getLG().isDead && (*it)->getLG().role != "Loup-Garou")
			goods++;
		if (role_vote == (*it)->getLG().role || role_vote == "all")
			(*it)->getLG().voted = false;
		(*it)->getLG().speech = speech;
		(*it)->getLG().votes = 0;
	}
	if (goods == 0)
	{
		for (std::vector<Client*>::iterator it = players.begin(); it != players.end();)
		{
			FormatIRC::sendNOTICE((*it)->getFd(), "#bot", ITALIC BOLD GREEN "Les loups garous gagnent" RESET);
			(*it)->getLG().isPlaying = false;
			players.erase(it);
		}
		return (false);
	}
	else if (bads == 0)
	{
		for (std::vector<Client*>::iterator it = players.begin(); it != players.end();)
		{
			FormatIRC::sendNOTICE((*it)->getFd(), "#bot", ITALIC BOLD GREEN "Les villageois gagnent" RESET);
			(*it)->getLG().isPlaying = false;
			players.erase(it);
		}
		return (false);
	}
	return (true);
}