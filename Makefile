SRCS = main.cpp ./server/sock.cpp ./request_response/Request.cpp ./request_response/Response.cpp

HEAD = ./server/sock.hpp ./request_response/Request.hpp ./request_response/Response.hpp

OBJS = $(SRCS:.cpp=.o)

CC = c++

CFLAGS =  -std=c++98

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