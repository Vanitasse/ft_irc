#include "../Bot.hpp"

void	Bot::LG_register(Client *client)
{
	t_LoupGarouPlayerID NewPlayerID;
	std::srand(time(NULL));

	NewPlayerID.isDead = false;
	NewPlayerID.isPlaying = true;
	NewPlayerID.voted = false;
	NewPlayerID.maire = false;
	NewPlayerID.votes = 0;
	NewPlayerID.speech = 2;
	if (rand() % 2 == 0)
		NewPlayerID.role = "Loup-Garou";
	else
		NewPlayerID.role = "Villageois";
	client->setLG(NewPlayerID);
	players.push_back(client);
	std::cout << players.size() << std::endl;
	FormatIRC::sendNOTICE(client->getFd(), "#bot", "Registered to Loup-Garou game");
	if (players.size() == 3)
	{
		phase = ElectionSpeechs;
		LG_Story_election();
	}
}

bool	Bot::LG_speech(Client *client, const std::string& msg)
{
	std::ostringstream oss;
	std::string format;
	int	total_speechs(0);

	if (client->getLG().speech == 0)
		return (FormatIRC::sendNOTICE(client->getFd(), "#bot", "You already used all your speech credits"), false);
	client->getLG().speech--;
	for (std::vector<Client*>::iterator it = players.begin(); it != players.end(); it++)
	{
		if ((*it)->getFd() != client->getFd())
			FormatIRC::sendPRIVMESS((*it)->getFd(), client->getNick(), this->_name, msg);
		total_speechs += (*it)->getLG().speech;
	}
	oss << client->getLG().speech;
	format = std::string("You stll have ") + oss.str() + std::string(" speech credit");
	FormatIRC::sendNOTICE(client->getFd(), "#bot", format);
	if (!total_speechs)
		return (true);
	return (false);
}

bool	Bot::LG_vote(Client *client, const std::string& msg, const std::string& role)
{
	bool	still_vote;

	if (client->getLG().voted)
		return (false);
	still_vote = false;
	if (client->getLG().role == role || role == "all")
	{
		for (std::vector<Client*>::iterator it = players.begin(); it != players.end(); it++)
		{
			if (msg == (*it)->getNick())
			{
				client->getLG().voted = true;
				(*it)->getLG().votes += 1 + client->getLG().maire;
			}
			if ((*it)->getLG().voted == false)
				still_vote = true;
		}
	}
	return (still_vote);
}

void	Bot::LG_newPhase(int speech)
{
	for (std::vector<Client*>::iterator it = players.begin(); it != players.end(); it++)
	{
		(*it)->getLG().speech = speech;
		(*it)->getLG().voted = false;
		(*it)->getLG().votes = 0;
	}
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
	Client *more_voted;

	if (client->getLG().isDead)
		FormatIRC::sendNOTICE(client->getFd(), "#bot", "You're dead");
	switch (phase)
	{
		case Connect:
			FormatIRC::sendNOTICE(client->getFd(), "#bot", "We need more players before launching the game, please wait");
			break;
		case ElectionSpeechs:
			if (LG_speech(client, msg))
			{
				phase = ElectionVotes;
				LG_newPhase(0);
				LG_Story_vote();
			}
			break;
		case ElectionVotes:
			if (LG_vote(client, msg, "all"))
			{
				more_voted = LG_getMoreVoted();
				if (more_voted == NULL)
				{
					FormatIRC::sendNOTICE(client->getFd(), "#bot", "le vote s'est solde par une egalite, votez de nouveau");
					LG_newPhase(0);
				}
				else
				{
					std::string format(more_voted->getNick() + " a ete elu maire du village");

					FormatIRC::sendNOTICE(client->getFd(), "#bot", format);
					more_voted->getLG().maire = true;
				}
				LG_newPhase(0);
				LG_Story_night();
				phase = Nuit;
			}
			break;
		case Nuit:
			if (LG_vote(client, msg, "Loup-Garou"))
			{
				more_voted = LG_getMoreVoted();
				if (more_voted == NULL)
				{
					FormatIRC::sendNOTICE(client->getFd(), "#bot", "le vote s'est solde par une egalite, votez de nouveau");
					LG_newPhase(0);
				}
				else
				{
					std::string format(more_voted->getNick() + " s'set fait bouffer");

					FormatIRC::sendNOTICE(client->getFd(), "#bot", format);
				}
				LG_newPhase(0);
				LG_Story_day();
				phase = Sorciere;
			}
			break;
		case Sorciere:
			if (LG_vote(client, msg, "Sorciere"))
			{
				LG_newPhase(3);
				phase = JourSpeech;
			}
			break;
		case JourSpeech:
			if (LG_speech(client, msg))
			{
				LG_newPhase(0);
				phase = JourVotes;
			}
			break;
		case JourVotes:
			if (LG_vote(client, msg, "all"))
			{
				phase = Nuit;
			}
			break;
	}
}

void	Bot::LG_Story_election()
{
	const std::string format("Villageois, l'heure est grave. Nous devons élire un maire pour nous guider face aux menaces qui planent sur notre village. Chacun d'entre vous peut se porter candidat. Ceux qui souhaitent diriger, faites un pas en avant et présentez-vous. Que le meilleur candidat gagne notre confiance et mène notre communauté avec sagesse. Le scrutin est ouvert. Votez pour celui ou celle qui, selon vous, mérite de devenir notre maire.");

	for (std::vector<Client*>::iterator it = players.begin(); it != players.end(); it++)
	{
		FormatIRC::sendNOTICE((*it)->getFd(), "#bot", format);
		FormatIRC::sendNOTICE((*it)->getFd(), "#bot", "You still have 2 speechs credits");
	}
}

void	Bot::LG_Story_vote()
{
	const std::string format("Le scrutin est ouvert. Votez pour celui ou celle qui, selon vous, mérite de devenir notre maire. (Ecrivez son nom)");

	for (std::vector<Client*>::iterator it = players.begin(); it != players.end(); it++)
	{
		FormatIRC::sendNOTICE((*it)->getFd(), "#bot", format);
	}
}

void	Bot::LG_Story_night()
{
	const std::string format("La nuit tombe sur Thiercelieux, et une obscurité maléfique engloutit le village. Les habitants se retirent dans leurs maisons, mais la peur les ronge, car quelque chose de terrible se tapit dans les ombres. Fermez les yeux et laissez le voile noir de la nuit vous envelopper...");

	for (std::vector<Client*>::iterator it = players.begin(); it != players.end(); it++)
	{
		FormatIRC::sendNOTICE((*it)->getFd(), "#bot", format);
	}
}

void	Bot::LG_Story_day()
{
	const std::string format("L'aube rouge s'éveille sur Thiercelieux, illuminant le village d'une lumière cruelle et sanglante. Ouvrez les yeux et contemplez l'œuvre des monstres nocturnes, alors que les ombres se retirent lentement.");

	for (std::vector<Client*>::iterator it = players.begin(); it != players.end(); it++)
	{
		FormatIRC::sendNOTICE((*it)->getFd(), "#bot", format);
	}
}