SRCS =  ./Src/main.cpp \
		./Src/Tools.cpp \
		./Src/Server/Server.cpp \
		./Src/Server/Poller.cpp \
		./Src/CGI/CGI.cpp \
		./Src/Config/Config.cpp \
		./Src/Config/Assigner.cpp \
		./Src//Config/Getters.cpp \
		./Src/Config/Tokens.cpp \
		./Src/http_protocol/HttpRequest.cpp \
		./Src/http_protocol/HttpHeaders.cpp \
		./Src/http_protocol/HttpRequestBuilder.cpp \
		./Src/http_protocol/HtppResponse.cpp \
		./Tests/Config/RunConfigTest.cpp \
		./Tests/CGI/CGI_test.cpp \

HEAD = ./Include/Server.hpp \
		./Include/Poller.hpp \
		./Include/Config.hpp \
		./Include/CGI.hpp \
		./Include/Tools.hpp \
		./Include/Types.hpp \
		./Include/HttpRequest.hpp \
		./Include/HttpHeaders.hpp \
		./Include/HttpResponse.hpp

OBJ_DIR = Objects

OBJS = $(patsubst %.cpp, $(OBJ_DIR)/%.o, $(SRCS))

CC = g++-10

RM = rm -f

CFLAGS = -std=c++98 -g

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
