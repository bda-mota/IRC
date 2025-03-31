# Special variables
DEFAULT_GOAL: all
.DELETE_ON_ERROR: $(NAME)
.PHONY: all bonus clean fclean re

#------------------------------------------------------------------------------#
#                                VARIABLES                                     #
#------------------------------------------------------------------------------#

# Compiler and flags
CC		=	c++
CFLAGS	=	-Wall -Werror -Wextra -std=c++98
RM		=	rm -rf

# Dir and file names
NAME	=	ircserv
SRCS	=	./src/main.cpp ./src/Server.cpp ./src/Client.cpp ./src/CommandParse.cpp ./src/Commands.cpp
BIN_DIR =	./bin
OBJS	=	$(addprefix $(BIN_DIR)/, $(notdir $(SRCS:.cpp=.o)))

# Colors
WHITE_U	=	"\033[37;1;4m"
WHITE	=	"\033[37;1m"
LIMITER	=	"\033[0m"
GREEN	=	"\033[92m"
RED		=	"\033[91m"
CYAN	=	"\033[96m"
YELLOW	=	"\033[93m"
BLUE	=	"\033[94m"
MAGENT	=	"\033[95m"

#------------------------------------------------------------------------------#
#                                 TARGETS                                      #
#------------------------------------------------------------------------------#

all: $(BIN_DIR) $(NAME)

# Generates output file
$(NAME): $(OBJS)
	@$(CC) $(CFLAGS) $(OBJS) -o $@

# Compiles sources into objects
$(BIN_DIR)/%.o: src/%.cpp
	@echo $(MAGENT)[Compiling $<]$(LIMITER) $(GREEN)with$(LIMITER) $(CYAN)[$(CC) $(CFLAGS)]$(LIMITER)
	@$(CC) $(CFLAGS) -c $< -o $@

# Create bin directory
$(BIN_DIR):
	@echo $(MAGENT)[Creating $(BIN_DIR)]$(LIMITER)
	@mkdir -p $@

# Removes objects
clean:
	@echo $(RED)[Cleaning objects]$(LIMITER)
	@$(RM) $(OBJS)

# Removes objects and executables
fclean: clean
	@echo $(RED)[Cleaning $(NAME)]$(LIMITER)
	@$(RM) $(NAME)
	@$(RM) $(BIN_DIR)

# Removes objects and executables and remakes
re: fclean all
