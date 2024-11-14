SRCS =  ./Src/main.cpp \
		./Src/Helper.cpp \
		./Src/Server/Server.cpp \
		./Src/Server/KqueueObj.cpp \
		./Src/Server/Client.cpp \
		./Src/Config/Config.cpp \
		./Src/Config/Assigner.cpp \
		./Src//Config/Checker.cpp \
		./Src/Config/Exps.cpp \
		./Src//Config/Getters.cpp \
		./Src/Config/Tokens.cpp \
		./Src/Config/Syntax.cpp \
		./Tests/Config/RunConfigTest.cpp \

HEAD = ./Include/Server.hpp \
		./Include/KqueueObj.hpp \
		./Include/Config.hpp \
		./Include/Client.hpp \
		./Include/Exps.hpp \
		./Include/Helper.hpp \
		./Include/Types.hpp \

OBJ_DIR = Objects

OBJS = $(patsubst %.cpp, $(OBJ_DIR)/%.o, $(SRCS))

CC = c++

RM = rm -f

CFLAGS = -std=c++98 -g -fsanitize=address

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
