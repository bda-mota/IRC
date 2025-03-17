#ifndef IRC_HPP
# define IRC_HPP

// colors
# define GREEN	"\033[32;1m"
# define RED	"\033[31;1m"
# define CYAN	"\033[36;1m"
# define BOLD	"\033[37;1;4m"
# define WHITE	"\033[37;1m"
# define YELLOW	"\033[33;1m"
# define BLUE	"\033[34;1m"
# define MAGENT	"\033[35;1m"
# define RESET	"\033[0m"

// libraries
# include <iostream>
# include <string>
# include <vector>
# include <unistd.h>
# include <sys/socket.h>  // Para funções socket(), bind(), listen(), accept()
# include <netinet/in.h>  // Para struct sockaddr_in e constantes como INADDR_ANY
# include <poll.h>  
# include <fcntl.h>
# include <signal.h>
# include <stdexcept>

// class
# include "./Client.hpp"
# include "./Server.hpp"

#endif