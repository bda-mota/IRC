#------------------------------------------------------------------------------#
#                                CONFIGURAÇÕES                                 #
#------------------------------------------------------------------------------#

PORT_NUMBER = 6667
PASSWORD = 123

# Compiler and flags
CC    = c++
FLAGS = -Wall -Werror -Wextra -std=c++98
RM    = rm -rf
NAME  = ircserv

# Directories
PATH_SRCS       = ./srcs/
PATH_COMMANDS     = ./srcs/commands/
PATH_CLASSES      = ./srcs/classes/
PATH_OBJS         = ./objs
PATH_OBJS_COMMANDS= $(PATH_OBJS)/commands
PATH_OBJS_CLASSES = $(PATH_OBJS)/classes

# Includes
INCLUDES = -I./includes

# Source files
FILES_SRC      = main.cpp utils.cpp
FILES_COMMANDS = USER.cpp NICK.cpp PASS.cpp QUIT.cpp JOIN.cpp \
				LISTC.cpp PRIVMSG.cpp KICK.cpp PART.cpp WHO.cpp MODE.cpp CAP.cpp\
				TOPIC.cpp INVITE.cpp auxiliaries.cpp mode_aux.cpp
FILES_CLASSES  = Server.cpp User.cpp Channel.cpp Command.cpp CommandsArgs.cpp

# Object files
OBJS_SRC      = $(addprefix $(PATH_OBJS)/, $(FILES_SRC:.cpp=.o))
OBJS_COMMANDS = $(addprefix $(PATH_OBJS_COMMANDS)/, $(FILES_COMMANDS:.cpp=.o))
OBJS_CLASSES  = $(addprefix $(PATH_OBJS_CLASSES)/, $(FILES_CLASSES:.cpp=.o))

# Colors
WHITE_U = "\033[37;1;4m"
WHITE = "\033[37;1m"
LIMITER = "\033[0m"
GREEN = "\033[92m"
RED   = "\033[91m"
CYAN  = "\033[96m"
YELLOW  = "\033[93m"
BLUE  = "\033[94m"
MAGENT  = "\033[95m"

#------------------------------------------------------------------------------#
#                                 TARGETS                                      #
#------------------------------------------------------------------------------#

all: $(NAME)

# Garante que os diretórios existem antes da compilação
$(NAME): | $(PATH_OBJS) $(PATH_OBJS_COMMANDS) $(PATH_OBJS_CLASSES)

# Compila o executável
$(NAME): $(OBJS_SRC) $(OBJS_CLASSES) $(OBJS_COMMANDS)
	@$(CC) $(FLAGS) $(OBJS_SRC) $(OBJS_CLASSES) $(OBJS_COMMANDS) -o $(NAME)
	@echo " "
	@echo "         \033[1;92m[SUCCESS] Compilation finished!\033[0m"
	@echo "\033[1;92m  ____  ____   ___  ___  ____  ____  _  _  ____  ____  \033[0m"
	@echo "\033[1;92m (_  _)(  _ \ / __)/ __)( ___)(  _ \( \/ )( ___)(  _ \ \033[0m"
	@echo "\033[1;92m  _)(_  )   /( (__ \__ \ )__)  )   / \  /  )__)  )   / \033[0m"
	@echo "\033[1;92m (____)(_)\_) \___)(___/(____)(_)\_)  \/  (____)(_)\_) \033[0m"

# Compila arquivos principais
$(PATH_OBJS)/%.o: $(PATH_SRCS)/%.cpp
	@mkdir -p $(dir $@)
	@echo -n "\033[1;92m. \033[0m"
	@$(CC) $(FLAGS) $(INCLUDES) -c $< -o $@

# Compila comandos
$(PATH_OBJS_COMMANDS)/%.o: $(PATH_COMMANDS)/%.cpp
	@mkdir -p $(dir $@)
	@echo -n "\033[1;92m. \033[0m"
	@$(CC) $(FLAGS) $(INCLUDES) -c $< -o $@

# Compila classes
$(PATH_OBJS_CLASSES)/%.o: $(PATH_CLASSES)/%.cpp
	@mkdir -p $(dir $@)
	@echo -n "\033[1;92m. \033[0m"
	@$(CC) $(FLAGS) $(INCLUDES) -c $< -o $@

# Executa o servidor
run: all
	@./$(NAME) $(PORT_NUMBER) $(PASSWORD)

# Executa com valgrind
valgrind: all
	valgrind --leak-check=full --track-origins=yes --show-leak-kinds=all ./$(NAME) $(PORT_NUMBER) $(PASSWORD)

# Limpa objetos
clean:
	@$(RM) $(PATH_OBJS)

# Limpa tudo
fclean: clean
	@$(RM) $(NAME)
	@echo "\033[1;91m ___  ____  ____  _  _  ____  ____    _____  ____  ____  __    ____  _  _  ____ \033[0m"
	@echo "\033[1;91m/ __)( ___)(  _ \( \/ )( ___)(  _ \  (  _  )( ___)( ___)(  )  (_  _)( \( )( ___)\033[0m"
	@echo "\033[1;91m\__ \ )__)  )   / \  /  )__)  )   /   )(_)(  )__)  )__)  )(__  _)(_  )  (  )__) \033[0m"
	@echo "\033[1;91m(___/(____)(_)\_)  \/  (____)(_)\_)  (_____)(__)  (__)  (____)(____)(_)\_)(____)\033[0m"

# Recompila tudo
re: fclean all

# Marca como alvos especiais
.PHONY: all clean fclean re run valgrind
.DEFAULT_GOAL := all
.DELETE_ON_ERROR: $(NAME)
