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
# include <limits>
# include <cstdlib>

# include "FormatIRC.hpp"


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


#define supp_info "CHANTYPES=# PREFIX=(o)@ MODES=1 CHANNELLEN=20 NICKLEN=20 TOPICLEN=200"

#define user_id(nickname, username) (":" + nickname + "!" + username + "@localhost")

#define bot_rules "Welcome ! I am totally not a bot so please treat me as one of you thanks. Here's some rules : To use my different commands, use a '!' If you ever feel lost, use !help and remember, stay safe out there.... fellow humans! ^^"

#define	StorySorciere0 "Réveille-toi, Sorcière. Devant toi se dresse la scène d’un massacre imminent. Une victime, choisie par les Loups-Garous, est déjà à demi dévorée, ses cris d’agonie étouffés par les ténèbres. Toi seule, avec ton pouvoir sombre et redoutable, peux décider de son sort. Tu disposes d'une potion de vie et d'une de mort, comptes tu les utiliser? (Entre le nom de la personne tuée cette nuit pour la sauver, ou un autre nom pour le tuer)"
#define	StorySorciere1 "Réveille-toi, Sorcière. Devant toi se dresse la scène d’un massacre imminent. Une victime, choisie par les Loups-Garous, est déjà à demi dévorée, ses cris d’agonie étouffés par les ténèbres. Toi seule, avec ton pouvoir sombre et redoutable, peux décider de son sort. Il te reste ta potion de vie, vas-tu l'utiliser? (Entre le nom de la personne tuée cette nuit pour la sauver)"
#define	StorySorciere2 "Réveille-toi, Sorcière. Devant toi se dresse la scène d’un massacre imminent. Une victime, choisie par les Loups-Garous, est déjà à demi dévorée, ses cris d’agonie étouffés par les ténèbres. Toi seule, avec ton pouvoir sombre et redoutable, peux décider de son sort. Il te reste ta potion de mort, vas-tu l'utiliser? (Entre un nom pour le debarassser d'un joueur)"
#define StoryLoupsGarous "Loups-Garous, réveillez-vous ! Choisissez votre victime parmi les âmes innocentes, dont le cœur bat encore. Sentez les battements d’un cœur terrifié à travers les murs fragiles de ce village maudit. Décidez qui goûtera à la morsure de vos crocs voraces cette nuit. Indiquez en silence celui ou celle dont le destin est scellé dans la terreur et la souffrance."
#define	StoryVoyante "La nuit enveloppe le village de son voile d'ombre. Tous les villageois sombrent dans un sommeil profond... sauf la Voyante. Celle qui perçoit l'invisible, réveille-toi. Tes yeux percent les ténèbres pour dévoiler la véritable nature de nos voisins. Montre-nous qui se cache derrière les masques. Choisis un villageois et découvre son secret. (entrez un nom)"
#define	StoryIntro "Dans “l’Est sauvage”, le petit hameau de Thiercelieux est depuis peu devenu la proie des Loups-Garous. Des meurtres sont commis chaque nuit par certains habitants du village, devenus Lycanthropes à cause d’un phénomène mystérieux (peut-être l’effet de serre ?)… Les Villageois doivent se ressaisir pour éradiquer ce nouveau fléau venu du fond des âges, avant que le hameau ne perde ses derniers habitants."
#define	StoryElection "Villageois, l'heure est grave. Nous devons élire un maire pour nous guider face aux menaces qui planent sur notre village. Chacun d'entre vous peut se porter candidat. Ceux qui souhaitent diriger, faites un pas en avant et présentez-vous. Que le meilleur candidat gagne notre confiance et mène notre communauté avec sagesse. Le scrutin est ouvert. Votez pour celui ou celle qui, selon vous, mérite de devenir notre maire."
#define	StoryDay "L'aube rouge s'éveille sur Thiercelieux, illuminant le village d'une lumière cruelle et sanglante. Ouvrez les yeux et contemplez l'œuvre des monstres nocturnes, alors que les ombres se retirent lentement."
#define	StoryNight "La nuit tombe sur Thiercelieux, et une obscurité maléfique engloutit le village. Les habitants se retirent dans leurs maisons, mais la peur les ronge, car quelque chose de terrible se tapit dans les ombres. Fermez les yeux et laissez le voile noir de la nuit vous envelopper..."
#define	StoryVoteMaire "Le scrutin est ouvert. Votez pour celui ou celle qui, selon vous, mérite de devenir notre maire. (Ecrivez son nom)"
#define StoryDayVote "Villageois, l'heure est grave. La nuit a été sombre, et des actes terribles ont été commis. Il est maintenant temps pour vous de décider qui, parmi vous, pourrait être un Loup-Garou. Chacun d'entre vous va pouvoir exprimer son vote et accuser la personne qu'il soupçonne. Souvenez-vous, vos vies en dépendent !"


#define election0 "  _______                      _           _                  _ "
#define election1 " |__   __|                    | |         | |                | |"
#define election2 "    | | ___  _   _ _ __    ___| | ___  ___| |_ ___  _ __ __ _| |"
#define election3 "    | |/ _ \\| | | | '__|  / _ \\ |/ _ \\/ __| __/ _ \\| '__/ _` | |"
#define election4 "    | | (_) | |_| | |    |  __/ |  __/ (__| || (_) | | | (_| | |"
#define election5 "    |_|\\___/ \\__,_|_|     \\___|_|\\___|\\___|\\__\\___/|_|  \\__,_|_|"

#define voteMaire0 " __      __   _                                      _                        _          "
#define voteMaire1 " \\ \\    / /  | |                                    | |                      (_)         "
#define voteMaire2 "  \\ \\  / /__ | |_ ___ ____  _ __   ___  _   _ _ __  | | ___   _ __ ___   __ _ _ _ __ ___ "
#define voteMaire3 "   \\ \\/ / _ \\| __/ _ \\_  / | '_ \\ / _ \\| | | | '__| | |/ _ \\ | '_ ` _ \\ / _` | | '__/ _ \\"
#define voteMaire4 "    \\  / (_) | ||  __// /  | |_) | (_) | |_| | |    | |  __/ | | | | | | (_| | | | |  __/"
#define voteMaire5 "     \\/ \\___/ \\__\\___/___| | .__/ \\___/ \\__,_|_|    |_|\\___| |_| |_| |_|\\__,_|_|_|  \\___|"
#define voteMaire6 "                           | |                                                           "
#define voteMaire7 "                           |_|                                                           "

#define voyante0 "  _______                     _        _        __      __                     _       "
#define voyante1 " |__   __|                   | |      | |       \\ \\    / /                    | |      "
#define voyante2 "    | | ___  _   _ _ __    __| | ___  | | __ _   \\ \\  / /__  _   _  __ _ _ __ | |_ ___ "
#define voyante3 "    | |/ _ \\| | | | '__|  / _` |/ _ \\ | |/ _` |   \\ \\/ / _ \\| | | |/ _` | '_ \\| __/ _ \\"
#define voyante4 "    | | (_) | |_| | |    | (_| |  __/ | | (_| |    \\  / (_) | |_| | (_| | | | | ||  __/"
#define voyante5 "    |_|\\___/ \\__,_|_|     \\__,_|\\___| |_|\\__,_|     \\/ \\___/ \\__, |\\__,_|_| |_|\\__\\___|"
#define voyante6 "                                                              __/ |                    "
#define voyante7 "                                                             |___/                     "

#define sorciere0 "  _______                     _        _          _____                _               "
#define sorciere1 " |__   __|                   | |      | |        / ____|              (_)              "
#define sorciere2 "    | | ___  _   _ _ __    __| | ___  | | __ _  | (___   ___  _ __ ___ _  ___ _ __ ___ "
#define sorciere3 "    | |/ _ \\| | | | '__|  / _` |/ _ \\ | |/ _` |  \\___ \\ / _ \\| '__/ __| |/ _ \\ '__/ _ \\"
#define sorciere4 "    | | (_) | |_| | |    | (_| |  __/ | | (_| |  ____) | (_) | | | (__| |  __/ | |  __/"
#define sorciere5 "    |_|\\___/ \\__,_|_|     \\__,_|\\___| |_|\\__,_| |_____/ \\___/|_|  \\___|_|\\___|_|  \\___|"
#define sorciere6 "                                                                                       "
#define sorciere7 "                                                                                       "

#define loupsGarous0 "  _______                     _             _                              _____                           "
#define loupsGarous1 " |__   __|                   | |           | |                            / ____|                          "
#define loupsGarous2 "    | | ___  _   _ _ __    __| | ___  ___  | |     ___  _   _ _ __  ___  | |  __  __ _ _ __ ___  _   _ ___ "
#define loupsGarous3 "    | |/ _ \\| | | | '__|  / _` |/ _ \\/ __| | |    / _ \\| | | | '_ \\/ __| | | |_ |/ _` | '__/ _ \\| | | / __|"
#define loupsGarous4 "    | | (_) | |_| | |    | (_| |  __/\\__ \\ | |___| (_) | |_| | |_) \\__ \\ | |__| | (_| | | | (_) | |_| \\__ \\"
#define loupsGarous5 "    |_|\\___/ \\__,_|_|     \\__,_|\\___||___/ |______\\___/ \\__,_| .__/|___/  \\_____|\\__,_|_|  \\___/ \\__,_|___/"
#define loupsGarous6 "                                                             | |                                           "
#define loupsGarous7 "                                                             |_|                                           "


#define villageois0 "  _______                     _             _ ___      ___ _ _                        _    _ _ "
#define villageois1 " |__   __|                   | |           ( | ) \\    / (_) | |                      (_)  ( | )"
#define villageois2 "    | | ___  _   _ _ __    __| | ___  ___   V V \\ \\  / / _| | | __ _  __ _  ___  ___  _ ___V V "
#define villageois3 "    | |/ _ \\| | | | '__|  / _` |/ _ \\/ __|       \\ \\/ / | | | |/ _` |/ _` |/ _ \\/ _ \\| / __|   "
#define villageois4 "    | | (_) | |_| | |    | (_| |  __/\\__ \\        \\  /  | | | | (_| | (_| |  __/ (_) | \\__ \\   "
#define villageois5 "    |_|\\___/ \\__,_|_|     \\__,_|\\___||___/         \\/   |_|_|_|\\__,_|\\__, |\\___|\\___/|_|___/   "
#define villageois6 "                                                                      __/ |                    "
#define villageois7 "                                                                     |___/                     "

#define coupable0 "  _______                                                                        _     _      "
#define coupable1 " |__   __|                                                                      | |   | |     "
#define coupable2 "    | |_ __ ___  _   ___   _____ ____  _   _ _ __     ___ ___  _   _ _ __   __ _| |__ | | ___ "
#define coupable3 "    | | '__/ _ \\| | | \\ \\ / / _ \\_  / | | | | '_ \\   / __/ _ \\| | | | '_ \\ / _` | '_ \\| |/ _ \\"
#define coupable4 "    | | | | (_) | |_| |\\ V /  __// /  | |_| | | | | | (_| (_) | |_| | |_) | (_| | |_) | |  __/"
#define coupable5 "    |_|_|  \\___/ \\__,_| \\_/ \\___/___|  \\__,_|_| |_|  \\___\\___/ \\__,_| .__/ \\__,_|_.__/|_|\\___|"
#define coupable6 "                                                                    | |                       "
#define coupable7 "                                                                    |_|                       "

typedef struct s_limits
{
	std::string				_chanType;
	std::string				_prefix;
	unsigned int			_nbModes;
	unsigned int			_channelLen;
	unsigned int			_nickLen;
	unsigned int			_topicLen;
}				t_limits;

typedef struct s_chanModes
{
	bool					_i;
	bool					_t;
	bool					_k;
	bool					_l;
}				t_chanModes;

enum	e_LoupGarouPhases
{
	Connect,
	ElectionSpeechs,
	ElectionVotes,
	Voyante,
	LoupsGarous,
	Sorciere,
	JourSpeech,
	JourVotes,
};


typedef struct	s_LoupGarouPlayerID
{
	int						ID;
	std::string				role;
	int						speech;
	bool					isPlaying;
	bool					isDead;
	bool					voted;
	bool					maire;
	int						votes;
}				t_LoupGarouPlayerID;