NAME = ircserv

SRCS = $(wildcard src/*.cpp src/network/*.cpp src/command/*.cpp)
OBJS = $(SRCS:.cpp=.o)

CC = c++
FLAGS =  -Wall -Wextra -Werror -std=c++98 # -Wall -Wextra -Werror
INCLUDES = -I ./includes

RM = rm -rf


.cpp.o:
	$(CC) $(FLAGS) $(INCLUDES) -c $< -o $(<:.cpp=.o)

all: $(NAME)

$(NAME): $(OBJS)
	@$(CC) $(FLAGS) $(INCLUDES) $(OBJS) -o $(NAME)

clean:
	@$(RM) $(OBJS)

fclean: clean
	@$(RM) $(NAME)

re: fclean all

.PHONY: all clean, fclean, re
