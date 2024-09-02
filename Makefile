SRCS = ./main.cpp ./server/Sock.cpp ./request_response/Request.cpp ./request_response/Response.cpp \
       ./gen_template/Generator.cpp ./utils/Convert.cpp ./server/Logger.cpp

HEAD = ./headers/Sock.hpp ./headers/Request.hpp ./headers/Response.hpp \
       ./headers/Generator.hpp ./headers/Utils.hpp ./headers/Structes.hpp ./headers/Logger.hpp \
# Directory for object files
OBJ_DIR = objects

# Adjust OBJS to put the .o files in the objects directory
OBJS = $(patsubst %.cpp, $(OBJ_DIR)/%.o, $(SRCS))

CC = c++

RM = rm -f

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
	$(RM) -r $(OBJ_DIR)

fclean: clean
	echo "Removing executable and libs..."
	$(RM) $(NAME)

re: fclean all

.PHONY: clean fclean re
