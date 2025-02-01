SRCS =	Src/main.cpp \
	Src/Tools.cpp \
	Src/Server/Server.cpp \
	Src/Server/Poller.cpp \
	Src/Server/Client.cpp \
	Src/CGI/CGI.cpp \
	Src/Config/Config.cpp \
	Src/Config/Assigner.cpp \
	Src//Config/Getters.cpp \
	Src/Config/Tokens.cpp \
	Src/Request/Request.cpp \
	Src/Request/HttpHeaders.cpp \
	Src/Request/Methods.cpp \
	Src/Request/UriValidator.cpp \
	Tests/Config/RunConfigTest.cpp
	# Tests/CGI/CGI_test.cpp \

HEAD = Include/Server.hpp \
		Include/Poller.hpp \
		Include/Config.hpp \
		Include/Client.hpp \
		Include/CGI.hpp \
		Include/Tools.hpp \
		Include/Types.hpp \
		Include/HttpRequest.hpp \
		Include/HttpHeaders.hpp \
		Include/HttpResponse.hpp \
		Include/UriValidator.hpp

OBJ_DIR = Objects

OBJS = $(patsubst %.cpp, $(OBJ_DIR)/%.o, $(SRCS))

CC = g++

RM = rm -f

CFLAGS = -std=c++98 -g #-fsanitize=address

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
