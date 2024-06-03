/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Irc.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mablatie <mablatie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/17 15:55:33 by bvaujour          #+#    #+#             */
/*   Updated: 2024/06/03 17:42:42 by mablatie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once


# include <iostream>
# include <vector>
# include <sys/socket.h>
# include <sys/types.h>
# include <netinet/in.h>
# include <fcntl.h>
# include <unistd.h>
# include <arpa/inet.h>
# include <poll.h>
# include <csignal>
# include <string.h>
# include <sstream>
# include <algorithm> 
# include <ctime>

// Smiley
# define SMILE "\U0001f604"

//ANSI

# define CLEAR "\033[2J\033[H"

// Effacer l'écran
#define CLEAR_SCREEN "\033[2J"
#define CLEAR_SCREEN_FROM_CURSOR "\033[0J"
#define CLEAR_SCREEN_TO_CURSOR "\033[1J"

// Effacer la ligne
#define CLEAR_LINE "\033[2K"
#define CLEAR_LINE_FROM_CURSOR "\033[0K"
#define CLEAR_LINE_TO_CURSOR "\033[1K"

// Positionnement du curseur
#define CURSOR_HOME "\033[H"
#define CURSOR_POSITION(row, col) "\033[" #row ";" #col "H"

// Déplacement relatif du curseur
#define CURSOR_UP(n) "\033[" #n "A"
#define CURSOR_DOWN(n) "\033[" #n "B"
#define CURSOR_FORWARD(n) "\033[" #n "C"
#define CURSOR_BACKWARD(n) "\033[" #n "D"

// Déplacement horizontal
#define CURSOR_HORIZONTAL_POSITION(n) "\033[" #n "G"

// Sauvegarder et restaurer la position du curseur
#define SAVE_CURSOR_POSITION "\033[s"
#define RESTORE_CURSOR_POSITION "\033[u"

// Styles de texte
#define RESET "\033[0m"
#define BOLD "\033[1m"
#define DIM "\033[2m"
#define ITALIC "\033[3m"
#define UNDERLINE "\033[4m"
#define BLINK_SLOW "\033[5m"
#define BLINK_FAST "\033[6m"
#define INVERSE "\033[7m"
#define HIDDEN "\033[8m"
#define STRIKETHROUGH "\033[9m"

// Couleurs de texte (premier plan)
#define FG_BLACK "\033[30m"
#define FG_RED "\033[31m"
#define FG_GREEN "\033[32m"
#define FG_YELLOW "\033[33m"
#define FG_BLUE "\033[34m"
#define FG_MAGENTA "\033[35m"
#define FG_CYAN "\033[36m"
#define FG_WHITE "\033[37m"
#define FG_BRIGHT_BLACK "\033[90m"
#define FG_BRIGHT_RED "\033[91m"
#define FG_BRIGHT_GREEN "\033[92m"
#define FG_BRIGHT_YELLOW "\033[93m"
#define FG_BRIGHT_BLUE "\033[94m"
#define FG_BRIGHT_MAGENTA "\033[95m"
#define FG_BRIGHT_CYAN "\033[96m"
#define FG_BRIGHT_WHITE "\033[97m"

// Couleurs de fond
#define BG_BLACK "\033[40m"
#define BG_RED "\033[41m"
#define BG_GREEN "\033[42m"
#define BG_YELLOW "\033[43m"
#define BG_BLUE "\033[44m"
#define BG_MAGENTA "\033[45m"
#define BG_CYAN "\033[46m"
#define BG_WHITE "\033[47m"
#define BG_BRIGHT_BLACK "\033[100m"
#define BG_BRIGHT_RED "\033[101m"
#define BG_BRIGHT_GREEN "\033[102m"
#define BG_BRIGHT_YELLOW "\033[103m"
#define BG_BRIGHT_BLUE "\033[104m"
#define BG_BRIGHT_MAGENTA "\033[105m"
#define BG_BRIGHT_CYAN "\033[106m"
#define BG_BRIGHT_WHITE "\033[107m"

// Masquer et afficher le curseur
#define HIDE_CURSOR "\033[?25l"
#define SHOW_CURSOR "\033[?25h"

// Couleurs de texte
#define RESET       "\033[0m"
#define BLACK       "\033[30m"      /* Noir */
#define RED         "\033[31m"      /* Rouge */
#define GREEN       "\033[32m"      /* Vert */
#define YELLOW      "\033[33m"      /* Jaune */
#define BLUE        "\033[34m"      /* Bleu */
#define MAGENTA     "\033[35m"      /* Magenta */
#define CYAN        "\033[36m"      /* Cyan */
#define WHITE       "\033[37m"      /* Blanc */

#define msg_serv(code, nickname) (":42IRCserv " + code + " " + nickname + " :")
#define msg_serv2(code, nickname) (":42IRCserv " + code + " " + nickname + " ")
#define msg_serv3(code, nickname, support_info) (":42IRCserv " + code + " " + nickname + " " + support_info + " :are supported by this server\r\n")

#define msg_err(code, nickname) (":42IRCserv " + code + " * " + nickname + " :")

#define supp_info() ("CHANTYPES=# PREFIX=(o)@ MODES=1 CHANLIMIT=#:5 NICKLEN=20 TOPICLEN=200")


#define user_id(nickname, username) (":" + nickname + "!" + username + "@localhost")
#define RPL_INVITE(user_id, invited, channel) (user_id + " INVITE " + invited + " #" + channel + "\r\n")
// [...]
#define ERR_INVALIDMODEPARAM(client, channel, mode, password) ("696 " + client + " #" + channel + " " + mode + " " + password + " : password must only contained alphabetic character\r\n")

enum	State
{
	LOGIN,
	PASSWORD,
	CONNECTED
};

enum	Destination
{
	DEFAULT,
	ANSWER_SENDER,
	SEND_CHAN
};