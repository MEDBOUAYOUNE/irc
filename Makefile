NAME = irc

SRC = ./main.cpp ./src/server.cpp ./src/utils.cpp #./src/client.cpp #./src/channel.cpp ./src/command.cpp ./src/logger.cpp 

CXX = c++ -std=c++98

CXXFLAGS = -Wall -Wextra -Werror

OBJ = $(SRC:.cpp=.o)

all: $(NAME)
$(NAME): $(OBJ)
	$(CXX) $(CXXFLAGS) $(OBJ) -o $(NAME)

clean:
	rm -rf $(OBJ)

fclean: clean
	rm -rf $(NAME)

re: fclean all