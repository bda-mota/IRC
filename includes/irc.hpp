#ifndef IRC_HPP
# define IRC_HPP

// libraries
# include <iostream>
# include <string>
# include <vector>
# include <map>
<<<<<<< HEAD
#include <sstream> // Para std::istringstream
#include <algorithm> // Para std::transform
=======
# include <algorithm>
>>>>>>> 16b594c92be1a07eb3cd45df5c45e4a6fae5c02e
# include <cstring>
# include <unistd.h>
# include <sys/socket.h>  // Para funções socket(), bind(), listen(), accept()
# include <netinet/in.h>  // Para struct sockaddr_in e constantes como INADDR_ANY
# include <poll.h>  
# include <fcntl.h>
# include <signal.h>
# include <stdexcept>
# include <arpa/inet.h>

// files includes 
# include "./User.hpp"
# include "./Server.hpp"
<<<<<<< HEAD
# include "./Command.hpp"
# include "./CommandsArgs.hpp"
=======
# include "./Channel.hpp"
# include "./Commands.hpp"
# include "./Macros.hpp"
>>>>>>> 16b594c92be1a07eb3cd45df5c45e4a6fae5c02e

#endif