#include "../Bot.hpp"

void	Bot::LG_register(Client *client)
{
	t_LoupGarouPlayerID NewPlayerID;
	std::size_t			nb_players(3);
	std::ostringstream	oss;

	std::srand(time(NULL));
	NewPlayerID.isDead = false;
	NewPlayerID.isPlaying = true;
	NewPlayerID.voted = false;
	NewPlayerID.maire = false;
	NewPlayerID.votes = 0;
	NewPlayerID.speech = 2;
	client->setLG(NewPlayerID);

	players.push_back(client);
	phase = Connect;
	bullet = 1;
	lifePotion = 1;
	deathPotion = 1;
	lastDead = NULL;

	FormatIRC::sendNOTICE(client->getFd(), "#bot", RED Titre1 RESET);
	FormatIRC::sendNOTICE(client->getFd(), "#bot", RED Titre2 RESET);
	FormatIRC::sendNOTICE(client->getFd(), "#bot", RED Titre3 RESET);
	FormatIRC::sendNOTICE(client->getFd(), "#bot", RED Titre4 RESET);
	FormatIRC::sendNOTICE(client->getFd(), "#bot", RED Titre5 RESET);
	FormatIRC::sendNOTICE(client->getFd(), "#bot", RED Titre6 RESET);
	FormatIRC::sendNOTICE(client->getFd(), "#bot", RED Titre7 RESET);
	FormatIRC::sendNOTICE(client->getFd(), "#bot", RED Titre8 RESET);
	FormatIRC::sendNOTICE(client->getFd(), "#bot", RED Titre9 RESET);
	FormatIRC::sendNOTICE(client->getFd(), "#bot", "Registered to Loup-Garou game");
	FormatIRC::sendNOTICE(client->getFd(), "#bot", "");
	FormatIRC::sendNOTICE(client->getFd(), "#bot", StoryIntro);
	if (nb_players - players.size())
	{
		oss << nb_players - players.size();
		LG_sendToPlayers("Waiting for " + oss.str() + " more players", "all");
	}
	if (players.size() == nb_players)
	{
		phase = ElectionSpeechs;
		LG_roles();
		LG_Story(StoryElection);
	}
}

void	Bot::LG_roles()
{
	int			random;
	std::size_t	sorciere(1);
	std::size_t	chasseur(1);
	std::size_t	cupidon(1);
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
				(*it)->getLG().role = "Sorciere";
			else if (random == 1 && chasseur == 1)
				(*it)->getLG().role = "Chasseur";
			else if (random == 2 && cupidon == 1)
				(*it)->getLG().role = "Cupidon";
			else if (random == 3 && voyante == 1)
				(*it)->getLG().role = "Voyante";
			else
				(*it)->getLG().role = "Villageois";
			villageois++;
		}
	}
	if (!loups_garous || !villageois)
		LG_roles();
}

void	Bot::LG_infos(Client *client)
{
	std::ostringstream oss;
	std::string format;

	format = std::string("Vous êtes un(e) ") + BLUE + client->getLG().role + RESET;
	FormatIRC::sendNOTICE(client->getFd(), "#bot", format);
	oss << client->getLG().speech;
	switch (phase)
	{
		case Connect:
			break;
		case ElectionSpeechs:
			format = "Vous avez " GREEN + oss.str() + RESET + " possibilié(s) de parler";
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
	for (std::vector<Client*>::iterator it = players.begin(); it != players.end(); it++)
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
	if (client->getNick() == msg && phase != Sorciere && phase != ElectionVotes)
		return (FormatIRC::sendNOTICE(client->getFd(), "#bot", "Vous ne pouvez pas voter pour vous même") ,false);
	if (phase == JourVotes && client->getLG().maire == true)
		vote_power = 2;
	if (client->getLG().role == role || role == "all")
	{
		for (std::vector<Client*>::iterator it = players.begin(); it != players.end(); it++)
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

void	Bot::LG_sendToPlayers(const std::string notice, const std::string role)
{
		for (std::vector<Client*>::iterator it = players.begin(); it != players.end(); it++)
		{
			if (role == "all" || (*it)->getLG().role == role)
				FormatIRC::sendNOTICE((*it)->getFd(), "#bot", notice);
		}
}

Client*	Bot::LG_findClientByRole(const std::string role)
{
	for (std::vector<Client*>::iterator it = players.begin(); it != players.end(); it++)
	{
		if ((*it)->getLG().role == role)
			return (*it);
	}
	return (NULL);
}

Client*	Bot::LG_getMoreVoted()
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

	if (client->getLG().isDead)
		return (FormatIRC::sendNOTICE(client->getFd(), "#bot", "Vous êtes mort, vous ne pouvez plus interagir avec les autres joueurs"));
	switch (phase)
	{
		case Connect:
			FormatIRC::sendNOTICE(client->getFd(), "#bot", "We need more players before launching the game, please wait");
			break;
		case ElectionSpeechs:
			if (LG_speech(client, msg))
			{
				phase = ElectionVotes;
				LG_newPhase(0, "all");
				LG_Story(StoryVoteMaire);
			}
			break;
		case ElectionVotes:
			if (LG_vote(client, msg, "all"))
			{
				tmp = LG_getMoreVoted();
				if (tmp == NULL)
				{
					LG_sendToPlayers("le vote s'est solde par une egalite, votez de nouveau", "all");
					LG_newPhase(0, "all");
				}
				else
				{
					LG_sendToPlayers(GREEN + tmp->getNick() + RESET + " a ete elu maire du village", "all");
					tmp->getLG().maire = true;
					LG_Story(StoryNight);
					tmp = LG_findClientByRole("Voyante");
					if (tmp && !tmp->getLG().isDead)
					{
						phase = Voyante;
						LG_newPhase(0, "Voyante");
						LG_Story(StoryVoyante);
					}
					else
					{
						LG_newPhase(0, "Loup-Garou");
						phase = LoupsGarous;
						LG_Story(StoryLoupsGarous);
					}
				}
			}
			break;
		case Voyante:
			if (LG_vote(client, msg, "Voyante"))
			{
				phase = LoupsGarous;
				tmp = LG_getMoreVoted();
				FormatIRC::sendNOTICE(client->getFd(), "#bot", "le role de ce joueur est " BLUE + tmp->getLG().role + RESET);
				LG_newPhase(0, "Loup-Garou");
				LG_Story(StoryLoupsGarous);
			}
			break;
		case LoupsGarous:
			if (LG_vote(client, msg, "Loup-Garou"))
			{
				lastDead = LG_getMoreVoted();
				if (lastDead == NULL)
				{
					LG_sendToPlayers("le vote s'est solde par une egalite, votez de nouveau", "Loup-Garou");
					LG_newPhase(0, "Loup-Garou");
				}
				else
				{
					lastDead->getLG().isDead = true;
					tmp = LG_findClientByRole("Voyante");
					if (tmp && !tmp->getLG().isDead && (lifePotion || deathPotion))
					{
						phase = Sorciere;
						if (lifePotion && deathPotion)
							LG_Story(StorySorciere0);
						else if (lifePotion)
							LG_Story(StorySorciere1);
						else if (deathPotion)
							LG_Story(StorySorciere2);
						FormatIRC::sendNOTICE(client->getFd(), "#bot", "le joueur " GREEN + lastDead->getNick() + RESET + " est mort cette nuit, que comptes tu faire?");
						LG_newPhase(0, "Sorciere");
					}
					else
					{
						phase = JourSpeech;
						LG_sendToPlayers("le joueur " GREEN + lastDead->getNick() + RESET + " est mort, son rôle etait " BLUE + lastDead->getLG().role, "all");
						LG_newPhase(3, "all");
						LG_Story(StoryDay);
					}
				}
			}
			break;
		case Sorciere:
			if (LG_vote(client, msg, "Sorciere"))
			{
				tmp = LG_getMoreVoted();
				if (tmp && (tmp->getNick() == lastDead->getNick()))
					lastDead->getLG().isDead = false;
				else if (tmp && (tmp->getNick() != lastDead->getNick()))
					tmp->getLG().isDead = true;
				phase = JourSpeech;
				LG_newPhase(3, "all");
			}
			break;
		case JourSpeech:
			if (LG_speech(client, msg))
			{
				phase = JourVotes;
				LG_newPhase(0, "all");
			}
			break;
		case JourVotes:
			if (LG_vote(client, msg, "all"))
			{
				lastDead = LG_getMoreVoted();
				if (lastDead == NULL)
				{
					LG_sendToPlayers("le vote s'est solde par une egalite, votez de nouveau", "all");
					LG_newPhase(0, "all");
				}
				else
				{
					lastDead->getLG().isDead = true;
					LG_sendToPlayers("le joueur " GREEN + lastDead->getNick() + RESET + " est pendu, son rôle etait " BLUE + lastDead->getLG().role, "all");
					if (LG_findClientByRole("Voyante"))
						{
							phase = Voyante;
							LG_newPhase(0, "Voyante");
							LG_Story(StoryVoyante);
						}
						else
						{
							phase = LoupsGarous;
							LG_newPhase(0, "Loup-Garou");
							LG_Story(StoryLoupsGarous);
						}
				}
			}
			break;
	}
}

void	Bot::LG_Story(const std::string story)
{

	for (std::vector<Client*>::iterator it = players.begin(); it != players.end(); it++)
	{
		switch (phase)
		{
			case Connect:
				break;
			case ElectionSpeechs:
				FormatIRC::sendNOTICE((*it)->getFd(), "#bot", BLUE jour1 RESET);
				FormatIRC::sendNOTICE((*it)->getFd(), "#bot", BLUE jour2 RESET);
				FormatIRC::sendNOTICE((*it)->getFd(), "#bot", BLUE jour3 RESET);
				FormatIRC::sendNOTICE((*it)->getFd(), "#bot", BLUE jour4 RESET);
				FormatIRC::sendNOTICE((*it)->getFd(), "#bot", BLUE jour5 RESET);
				FormatIRC::sendNOTICE((*it)->getFd(), "#bot", BLUE jour6 RESET);
				FormatIRC::sendNOTICE((*it)->getFd(), "#bot", BLUE jour7 RESET);
				FormatIRC::sendNOTICE((*it)->getFd(), "#bot", GREEN election1 RESET);
				FormatIRC::sendNOTICE((*it)->getFd(), "#bot", GREEN election2 RESET);
				FormatIRC::sendNOTICE((*it)->getFd(), "#bot", GREEN election3 RESET);
				FormatIRC::sendNOTICE((*it)->getFd(), "#bot", GREEN election4 RESET);
				FormatIRC::sendNOTICE((*it)->getFd(), "#bot", GREEN election5 RESET);
				break;
			case ElectionVotes:
				FormatIRC::sendNOTICE((*it)->getFd(), "#bot", BLUE jour1 RESET);
				FormatIRC::sendNOTICE((*it)->getFd(), "#bot", BLUE jour2 RESET);
				FormatIRC::sendNOTICE((*it)->getFd(), "#bot", BLUE jour3 RESET);
				FormatIRC::sendNOTICE((*it)->getFd(), "#bot", BLUE jour4 RESET);
				FormatIRC::sendNOTICE((*it)->getFd(), "#bot", BLUE jour5 RESET);
				FormatIRC::sendNOTICE((*it)->getFd(), "#bot", BLUE jour6 RESET);
				FormatIRC::sendNOTICE((*it)->getFd(), "#bot", BLUE jour7 RESET);
				break;
			case Voyante:
				FormatIRC::sendNOTICE((*it)->getFd(), "#bot", RED nuit1 RESET);
				FormatIRC::sendNOTICE((*it)->getFd(), "#bot", RED nuit2 RESET);
				FormatIRC::sendNOTICE((*it)->getFd(), "#bot", RED nuit3 RESET);
				FormatIRC::sendNOTICE((*it)->getFd(), "#bot", RED nuit4 RESET);
				FormatIRC::sendNOTICE((*it)->getFd(), "#bot", RED nuit5 RESET);
				FormatIRC::sendNOTICE((*it)->getFd(), "#bot", RED nuit6 RESET);
				FormatIRC::sendNOTICE((*it)->getFd(), "#bot", RED nuit7 RESET);
				break;
			case LoupsGarous:
				FormatIRC::sendNOTICE((*it)->getFd(), "#bot", RED nuit1 RESET);
				FormatIRC::sendNOTICE((*it)->getFd(), "#bot", RED nuit2 RESET);
				FormatIRC::sendNOTICE((*it)->getFd(), "#bot", RED nuit3 RESET);
				FormatIRC::sendNOTICE((*it)->getFd(), "#bot", RED nuit4 RESET);
				FormatIRC::sendNOTICE((*it)->getFd(), "#bot", RED nuit5 RESET);
				FormatIRC::sendNOTICE((*it)->getFd(), "#bot", RED nuit6 RESET);
				FormatIRC::sendNOTICE((*it)->getFd(), "#bot", RED nuit7 RESET);
				break;
			case Sorciere:
				FormatIRC::sendNOTICE((*it)->getFd(), "#bot", RED nuit1 RESET);
				FormatIRC::sendNOTICE((*it)->getFd(), "#bot", RED nuit2 RESET);
				FormatIRC::sendNOTICE((*it)->getFd(), "#bot", RED nuit3 RESET);
				FormatIRC::sendNOTICE((*it)->getFd(), "#bot", RED nuit4 RESET);
				FormatIRC::sendNOTICE((*it)->getFd(), "#bot", RED nuit5 RESET);
				FormatIRC::sendNOTICE((*it)->getFd(), "#bot", RED nuit6 RESET);
				FormatIRC::sendNOTICE((*it)->getFd(), "#bot", RED nuit7 RESET);
				break;
			case JourSpeech:
				FormatIRC::sendNOTICE((*it)->getFd(), "#bot", BLUE jour1 RESET);
				FormatIRC::sendNOTICE((*it)->getFd(), "#bot", BLUE jour2 RESET);
				FormatIRC::sendNOTICE((*it)->getFd(), "#bot", BLUE jour3 RESET);
				FormatIRC::sendNOTICE((*it)->getFd(), "#bot", BLUE jour4 RESET);
				FormatIRC::sendNOTICE((*it)->getFd(), "#bot", BLUE jour5 RESET);
				FormatIRC::sendNOTICE((*it)->getFd(), "#bot", BLUE jour6 RESET);
				FormatIRC::sendNOTICE((*it)->getFd(), "#bot", BLUE jour7 RESET);
				break;
			case JourVotes:
				FormatIRC::sendNOTICE((*it)->getFd(), "#bot", BLUE jour1 RESET);
				FormatIRC::sendNOTICE((*it)->getFd(), "#bot", BLUE jour2 RESET);
				FormatIRC::sendNOTICE((*it)->getFd(), "#bot", BLUE jour3 RESET);
				FormatIRC::sendNOTICE((*it)->getFd(), "#bot", BLUE jour4 RESET);
				FormatIRC::sendNOTICE((*it)->getFd(), "#bot", BLUE jour5 RESET);
				FormatIRC::sendNOTICE((*it)->getFd(), "#bot", BLUE jour6 RESET);
				FormatIRC::sendNOTICE((*it)->getFd(), "#bot", BLUE jour7 RESET);
		}
		FormatIRC::sendNOTICE((*it)->getFd(), "#bot", "");
		FormatIRC::sendNOTICE((*it)->getFd(), "#bot", story);
		FormatIRC::sendNOTICE((*it)->getFd(), "#bot", "");
		LG_infos(*it);
	}
}

void	Bot::LG_newPhase(int speech, const std::string& role_vote)
{
	for (std::vector<Client*>::iterator it = players.begin(); it != players.end(); it++)
	{
		if ((*it)->getLG().isDead)
		{
			(*it)->getLG().voted = true;
			(*it)->getLG().speech = 0;
			(*it)->getLG().votes = 0;
		}
		if (role_vote == (*it)->getLG().role || role_vote == "all")
			(*it)->getLG().voted = false;
		(*it)->getLG().speech = speech;
		(*it)->getLG().votes = 0;
	}
}