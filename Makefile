SRCS =	Src/main.cpp \
	Src/Tools.cpp \
	Src/Config/Config.cpp \
	Src/Config/Assigner.cpp \
	Src/Config/Tokens.cpp \
	Src/Config/ConfigFileStorage.cpp \
	Src/Config/ServerStorage.cpp \
	Src/Config/RouteStorage.cpp

HEAD =  Include/Config.hpp \
		Include/Tools.hpp \
		Include/Types.hpp \
		Include/Storage.hpp

OBJ_DIR = Objects

OBJS = $(patsubst %.cpp, $(OBJ_DIR)/%.o, $(SRCS))

CC = c++

RM = rm -f

CFLAGS = -std=c++98 -Wall -Wextra -Werror -g -fsanitize=address

NAME = webserv

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) -o $(NAME) $(OBJS)

$(OBJ_DIR)/%.o: %.cpp $(HEAD)
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@


clean:
	$(RM) $(OBJS)
	$(RM) -r $(OBJ_DIR)

fclean: clean
	$(RM) $(NAME)

re: fclean all
test:
	python3 tester.py --host 127.0.0.1 --port 4448 --clients 100

.PHONY: clean
