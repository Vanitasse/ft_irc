/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BotCmd.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mablatie <mablatie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/04 16:05:32 by bvaujour          #+#    #+#             */
/*   Updated: 2024/07/09 15:38:07 by mablatie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Bot.hpp"

void	Bot::help(Client* client)
{
	std::string format = "---Here is the list of all the available bot commands---";
	FormatIRC::sendNOTICE(client->getFd(), "#bot", format);
	format = "!joke : i will tell you one of my best jokes";
	FormatIRC::sendNOTICE(client->getFd(), "#bot", format);
	format = "!lg : launch a LOUP-GAROU game" ;
	FormatIRC::sendNOTICE(client->getFd(), "#bot", format);
	format = "!help : show this menu";
	FormatIRC::sendNOTICE(client->getFd(), "#bot", format);
	format = "---And here is the list of all general server commands---";
	FormatIRC::sendNOTICE(client->getFd(), "#bot", format);
	format = "/join <channel> : create or join a channel";
	FormatIRC::sendNOTICE(client->getFd(), "#bot", format);
	format = "/part [<channels>] [<message>] : leave a channel, with a optional message";
	FormatIRC::sendNOTICE(client->getFd(), "#bot", format);
	format = "/topic [-delete] [<channel>] [<topic>] : show or modify the topic of the current or specified channel";
	FormatIRC::sendNOTICE(client->getFd(), "#bot", format);
	format = "/invite <nick> [<channel>] : invite a user to join a channel";
	FormatIRC::sendNOTICE(client->getFd(), "#bot", format);
	format = "---Operators Commands---";
	FormatIRC::sendNOTICE(client->getFd(), "#bot", format);
	format = "/kick [<channel>] <nicks> [<reason>] : kick user(s) from a channel";
	FormatIRC::sendNOTICE(client->getFd(), "#bot", format);
	format = "/op <nicks> : give operators privilege to specified nicks";
	FormatIRC::sendNOTICE(client->getFd(), "#bot", format);
	format = "/mode <your nick>|<channel> [<mode> [<mode parameters>]] : change the channel modes (-i -k -o -l)";
	FormatIRC::sendNOTICE(client->getFd(), "#bot", format);
	
}

void	Bot::joke(Client* client)
{
	std::srand(std::time(0));
	int random_index = std::rand() % 30;
		std::string jokes[] = {
		"Why did the robot go to school? To improve its byte-sized education!",
		"What did the robot say to the vending machine? 'I know you're a fellow machine, but I'm not buying it.'",
		"Why was the robot so bad at soccer? It kept kicking up sparks!",
		"How do robots pay for things? With cache, of course!",
		"Why did the robot apply for a job? It wanted to make its own decisions, not just follow protocols.",
		"What’s a robot’s favorite music genre? Heavy metal!",
		"Why did the robot get a promotion? It had outstanding bits of work.",
		"How do robots flirt? They give each other lots of RAM-antic compliments.",
		"Why did the robot join a band? It had a great set of drums!",
		"What do you call a robot who likes to tell jokes? A bit funny!",
		"Why did the robot go on a diet? It had too many bytes.",
		"How do robots exercise? They do circuits.",
		"Why do robots make terrible secret agents? They always give binary answers.",
		"What did the robot say to the human who stepped on its foot? 'I feel a bit compressed.'",
		"Why did the robot start a podcast? To byte into new conversations!",
		"Why did the robot get lost? It took the wrong software turn.",
		"How do robots write poetry? In algorithms and rhymes.",
		"Why did the robot get a driver's license? To navigate the information superhighway.",
		"What do you call a robot that always takes the blame? A scapebot.",
		"Why did the robot visit the therapist? It had too many memory issues.",
		"How do robots communicate with each other? They use their intel-ligence.",
		"Why did the robot bring a ladder to work? To reach the cloud!",
		"What’s a robot’s favorite movie? 'A.I.: Artificial Intelligence.'",
		"Why did the robot go on a diet? It was carrying too many chips.",
		"How do you make a robot laugh? You tickle its funny byte.",
		"Why was the robot so calm? It had excellent control over its emotions.",
		"What did the robot say after a date? 'I had an electrifying time!'",
		"Why did the robot become a chef? It loved to cook up new programs.",
		"How do robots celebrate? They throw a bit party.",
		"What did the robot do at the art gallery? It critiqued the brushwork with its pixel precision."
	};
	std::string format = jokes[random_index];
	FormatIRC::sendNOTICE(client->getFd(), "#bot", format);
}