NAME	=	ircserv

SRC_DIR	=	./src/

INCLUDE	=	Server.hpp Irc.hpp Client.hpp Channel.hpp FormatIRC.hpp

OBJ_DIR	=	./obj/

FILES	=	main.cpp		\
			Client.cpp		\
			ClientCmd.cpp	\
			Server.cpp		\
			ServerUtils.cpp	\
			Channel.cpp		\
			FormatIRC.cpp	\
			# Bot.cpp			\


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
	rm *.log

re: fclean all

.PHONY: all clean fclean re