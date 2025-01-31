SRCS =  ./Src/main.cpp \
		./Src/Tools.cpp \
		./Src/Server/Server.cpp \
		./Src/Server/Poller.cpp \
		./Src/http_protocol/Request.cpp \
		./Src/http_protocol/Response.cpp \
		./Src/Config/Config.cpp \
		./Src/Config/Assigner.cpp \
		./Src//Config/Getters.cpp \
		./Src/Config/Tokens.cpp \
		./Src/CGI/CGI.cpp

HEAD = ./Include/Server.hpp \
		./Include/Poller.hpp \
		./Include/Config.hpp \
		./Include/Tools.hpp \
		./Include/Types.hpp \
		./Include/Request.hpp \
		./Include/Response.hpp \
		./Include/CGI.hpp

OBJ_DIR = Objects

OBJS = $(patsubst %.cpp, $(OBJ_DIR)/%.o, $(SRCS))

CC = g++-11

RM = rm -f

CFLAGS = -std=c++98 -g -o0 -fsanitize=address

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
