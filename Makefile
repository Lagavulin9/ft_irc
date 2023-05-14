NAME = ircserv
CXX = c++
CXXFLAGS = -Wall -Werror -Wextra -std=c++98 #-fsanitize=address -g3
LDFLAGS = 
INC = includes
SRC_DIR = srcs/
SRC = main.cpp \
	Server.cpp \
	Channel.cpp \
	Client.cpp \
	Utils.cpp \
	RPL.cpp
SRCS = $(addprefix $(SRC_DIR), $(SRC))
OBJS = $(SRCS:.cpp=.o)

ifdef SANITIZE
	CXXFLAGS = -Wall -Werror -Wextra -std=c++98 -fsanitize=address -g3
endif

all : $(NAME)

$(NAME) : $(OBJS)
	$(CXX) $(CXXFLAGS) -I $(INC) $(OBJS) -o $(NAME)

clean :
	rm -rf $(OBJS)

fclean : clean
	rm -rf $(NAME)

re : fclean all

sanitize : fclean
	make SANITIZE=1 all

.PHONY: all clean fclean re sanitize