SRCS = main.cpp ./server/sock.cpp ./request_response/Request.cpp ./request_response/Response.cpp \
       ./gen_template/Generator.cpp ./utils/convert.cpp ./server/Logger.cpp

HEAD = ./server/sock.hpp ./request_response/Request.hpp ./request_response/Response.hpp \
       ./gen_template/Generator.hpp ./utils/utils.hpp ./structes.hpp ./server/Logger.hpp

# Directory for object files
OBJ_DIR = objects

# Adjust OBJS to put the .o files in the objects directory
OBJS = $(patsubst %.cpp, $(OBJ_DIR)/%.o, $(SRCS))

CC = c++

CFLAGS = -std=c++98

NAME = webserv

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) -o $(NAME) $(OBJS)

# Rule to compile .cpp files to .o files in the objects directory
$(OBJ_DIR)/%.o: %.cpp $(HEAD)
	@mkdir -p $(dir $@)
	echo "Building $<..."
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	echo "Removing all object files..."
	$(RM) $(OBJS)

fclean: clean
	echo "Removing executable and libs..."
	$(RM) $(NAME)

re: fclean all

.PHONY: clean fclean re
