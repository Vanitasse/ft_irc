/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Irc.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mablatie <mablatie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/17 15:55:33 by bvaujour          #+#    #+#             */
/*   Updated: 2024/05/28 17:51:13 by mablatie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once


# include <iostream>
# include <vector>
# include <sys/socket.h> //-> for socket()
# include <sys/types.h> //-> for socket() options
# include <netinet/in.h> //-> for sockaddr_in
# include <fcntl.h> //-> for fcntl()
# include <unistd.h> //-> for close()
# include <arpa/inet.h> //-> for inet_ntoa()
# include <poll.h> //-> for poll()
# include <csignal> //-> for signal()
# include <string.h>
# include <sstream>

// Smiley
# define SMILE "\U0001f604"

//ANSI

# define CLEAR "\033[2J\033[H"

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

// Couleurs de fond
#define BG_BLACK    "\033[40m"      /* Fond Noir */
#define BG_RED      "\033[41m"      /* Fond Rouge */
#define BG_GREEN    "\033[42m"      /* Fond Vert */
#define BG_YELLOW   "\033[43m"      /* Fond Jaune */
#define BG_BLUE     "\033[44m"      /* Fond Bleu */
#define BG_MAGENTA  "\033[45m"      /* Fond Magenta */
#define BG_CYAN     "\033[46m"      /* Fond Cyan */
#define BG_WHITE    "\033[47m"      /* Fond Blanc */

// Styles de texte
#define BOLD        "\033[1m"       /* Gras */
#define ITALIC      "\033[3m"       /* Italique */
#define UNDERLINE   "\033[4m"       /* Souligné */
#define INVERSE     "\033[7m"       /* Inverser les couleurs */

enum	State
{
	LOGIN,
	PASSWORD,
	CONNECTED
};