# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: mablatie <mablatie@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/05/17 15:03:53 by bvaujour          #+#    #+#              #
#    Updated: 2024/05/28 15:20:26 by mablatie         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME	=	ircserv

SRC_DIR	=	./src/

INCLUDE	=	Irc.hpp Client.hpp Server.hpp Channel.hpp

OBJ_DIR	=	./obj/

FILES	=	main.cpp		\
			Client.cpp		\
			ClientCore.cpp	\
			Server.cpp		\
			ServerUtils.cpp	\
			ServerCore.cpp	\
			Channel.cpp		\

SRC		= $(addprefix $(SRC_DIR), $(FILES))

OBJ		= $(addprefix $(OBJ_DIR), $(FILES:.cpp=.o))

CC		= c++

FLAGS = -Wall -Werror -Wextra -g

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