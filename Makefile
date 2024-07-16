SRCS = main.cpp ./server/sock.cpp

HEAD = ./server/sock.hpp

OBJS = $(SRCS:.cpp=.o)

CC = c++

CFLAGS = -Wall -Wextra -Werror -std=c++98

NAME = webserv

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) -o $(NAME) $(OBJS)

%.o: %.cpp $(HEAD)
	echo "Building $<..."
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	echo "Removing all object files..."
	$(RM) $(OBJS)

fclean: clean
	@echo "Removing executable and libs..."
	$(RM) $(NAME)

re: fclean all

.PHONY: clean

.SILENT: