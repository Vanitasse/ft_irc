# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: bvaujour <bvaujour@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/05/17 15:03:53 by bvaujour          #+#    #+#              #
#    Updated: 2024/06/01 19:09:07 by bvaujour         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME	=	ircserv

SRC_DIR	=	./src/

INCLUDE	=	Irc.hpp Client.hpp Server.hpp IrssiClient.hpp NcClient.hpp

OBJ_DIR	=	./obj/

FILES	=	main.cpp		\
			Client.cpp		\
			Server.cpp		\
			ServerUtils.cpp	\
			IrssiClient.cpp	\
			NcClient.cpp	\

SRC		= $(addprefix $(SRC_DIR), $(FILES))

OBJ		= $(addprefix $(OBJ_DIR), $(FILES:.cpp=.o))

CC		= c++

FLAGS = -Wall -Werror -Wextra

$(OBJ_DIR)%.o: $(SRC_DIR)%.cpp $(INCLUDE)
	mkdir -p $(OBJ_DIR)
	$(CC) $(FLAGS) -c $< -o $@

all: $(NAME)

$(NAME): $(OBJ) Makefile
	$(CC) $(FLAGS) $(OBJ) -o $(NAME)


clean:
	rm -rf $(OBJ_DIR) 

fclean: clean
	rm -rf $(NAME)

re: fclean all

.PHONY: all clean fclean re