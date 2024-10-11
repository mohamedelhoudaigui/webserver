SRCS = ./Src/main.cpp ./Src/Server.cpp ./Src/Request.cpp

HEAD = ./Include/Server.hpp ./Include/Request.hpp 
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
