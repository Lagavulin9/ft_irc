NAME = ircserv
CXX = c++
CXXFLAGS = -Wall -Werror -Wextra -std=c++98
LDFLAGS = 
INC = includes
SRC_DIR = srcs/
SRC = main.cpp \
	Server.cpp
SRCS = $(addprefix $(SRC_DIR), $(SRC))
OBJS = $(SRCS:.cpp=.o)

all : $(NAME)

$(NAME) : $(OBJS)
	$(CXX) $(CXXFLAGS) -I $(INC) $(OBJS) -o $(NAME)

clean :
	rm -rf $(OBJS)

fclean : clean
	rm -rf $(NAME)

re : fclean all

.PHONY: all clean fclean re