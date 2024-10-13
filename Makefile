SRCS = ./Src/main.cpp ./Src/Server.cpp ./Src/Request.cpp ./Src/Response.cpp ./Src/Config.cpp ./Src/Helper.cpp

HEAD = ./Include/Server.hpp \
		./Include/Request.hpp \
		./Include/Response.hpp \
		./Include/Config.hpp \
		./Include/Helper.hpp \
		./Include/Types.hpp \

OBJ_DIR = Objects

OBJS = $(patsubst %.cpp, $(OBJ_DIR)/%.o, $(SRCS))

CC = c++

RM = rm -f

CFLAGS = -std=c++98

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

.PHONY: clean
