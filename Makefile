# Special variables
.DEFAULT_GOAL: all
.DELETE_ON_ERROR: $(NAME)
.PHONY: all clean fclean re

PORT_NUMBER = 6667
PASSWORD = "password"

#------------------------------------------------------------------------------#
#                                VARIABLES                                     #
#------------------------------------------------------------------------------#

# Compiler and flags
CC		=	c++
FLAGS	=	-Wall -Werror -Wextra -std=c++98
RM		=	rm -rf
NAME	=	ircserv

# Dir and files
PATH_SRCS	  =	./srcs/
PATH_COMMANDS = ./srcs/commands/
PATH_CLASSES  = ./srcs/classes/
PATH_OBJS	  =	./objs/

INCLUDES = -I./includes
FILES_SRC = main.cpp
FILES_COMMANDS = JOIN.cpp PRIVMSG.cpp TOPIC.cpp auxiliaries.cpp
FILES_CLASSES = Server.cpp User.cpp Channel.cpp Command.cpp CommandsArgs.cpp


# objects files
OBJS = $(addprefix $(PATH_OBJS), $(FILES_SRC:.cpp=.o))
OBJS_COMMANDS = $(addprefix $(PATH_OBJS), $(FILES_COMMANDS:.cpp=.o))
OBJS_CLASSES = $(addprefix $(PATH_OBJS), $(FILES_CLASSES:.cpp=.o))

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

# Default target to build the project
all: $(NAME)

run: all
	@./$(NAME) $(PORT_NUMBER) $(PASSWORD)
	
# Creates the executable
$(NAME): $(OBJS) $(OBJS_CLASSES) $(OBJS_COMMANDS)
	@$(CC) $(FLAGS) $(OBJS) $(OBJS_CLASSES) $(OBJS_COMMANDS) -o $(NAME)
	@echo " "
	@echo "         \033[1;92m[SUCCESS] Compilation finished!\033[0m"
	@echo "\033[1;92m  ____  ____   ___  ___  ____  ____  _  _  ____  ____  \033[0m"
	@echo "\033[1;92m (_  _)(  _ \ / __)/ __)( ___)(  _ \( \/ )( ___)(  _ \ \033[0m"
	@echo "\033[1;92m  _)(_  )   /( (__ \__ \ )__)  )   / \  /  )__)  )   /\033[0m"
	@echo "\033[1;92m (____)(_)\_) \___)(___/(____)(_)\_)  \/  (____)(_)\_)\033[0m"


# Rule to compile object files
$(PATH_OBJS)%.o: $(PATH_SRCS)%.cpp | $(PATH_OBJS)
	@echo -n "\033[1;92m. \033[0m"
	@$(CC) $(FLAGS) $(INCLUDES) -c $< -o $@

$(PATH_OBJS)%.o: $(PATH_CLASSES)%.cpp | $(PATH_OBJS)
	@echo -n "\033[1;92m. \033[0m"
	@$(CC) $(FLAGS) $(INCLUDES) -c $< -o $@

$(PATH_OBJS)%.o: $(PATH_COMMANDS)%.cpp | $(PATH_OBJS)
	@echo -n "\033[1;92m. \033[0m"
	@$(CC) $(FLAGS) $(INCLUDES) -c $< -o $@

# Create the objs directory if it doesn't exist
$(PATH_OBJS): 
	@mkdir -p $(PATH_OBJS)

# Removes object files
clean:
	@$(RM) $(PATH_OBJS)

# Removes objects and executables
fclean: clean
	@$(RM) $(NAME)
	@$(RM) -r $(PATH_OBJS)
	@echo "\033[1;91m ___  ____  ____  _  _  ____  ____    _____  ____  ____  __    ____  _  _  ____ \033[0m"
	@echo "\033[1;91m/ __)( ___)(  _ \( \/ )( ___)(  _ \  (  _  )( ___)( ___)(  )  (_  _)( \( )( ___)\033[0m"
	@echo "\033[1;91m\__ \ )__)  )   / \  /  )__)  )   /   )(_)(  )__)  )__)  )(__  _)(_  )  (  )__) \033[0m"
	@echo "\033[1;91m(___/(____)(_)\_)  \/  (____)(_)\_)  (_____)(__)  (__)  (____)(____)(_)\_)(____)\033[0m"

# Removes objects and executables and remakes
re: fclean all
