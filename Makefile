NAME		:= ircserv
CPPFLAGS	:= -Wall -Wextra -Werror -std=c++98 -Wshadow -fsanitize=address
SRCS		:= main.cpp cmd.cpp init.cpp clientInfo.cpp mode.cpp Channel.cpp Server.cpp
CXX 		:= c++
OBJ			:= ${SRCS:%.cpp=%.o}

all: $(NAME)

$(NAME): $(OBJ)
	${CXX} $(CPPFLAGS) -o $(NAME) $(OBJ)

clean:
	@rm -rf $(OBJ)

fclean: clean
	@rm -rf $(NAME)

re: clean all

.PHONY: all, clean, fclean, re