#ifndef IRC_HPP
# define IRC_HPP

// libraries
# include <iostream>
# include <string>
# include <vector>
# include <map>
# include <set>
# include <sstream> // Para std::istringstream
# include <algorithm> // Para std::transform
# include <cstring>
# include <unistd.h>
# include <sys/socket.h>  // Para funções socket(), bind(), listen(), accept()
# include <netinet/in.h>  // Para struct sockaddr_in e constantes como INADDR_ANY
# include <poll.h>  
# include <fcntl.h>
# include <signal.h>
# include <stdexcept>
# include <arpa/inet.h>
# include <ctime>    // para std::time, std::time_t, std::localtime
# include <iomanip>  // para std::setw, std::setfill

// files includes 
# include "./User.hpp"
# include "./Server.hpp"
# include "./Command.hpp"
# include "./CommandsArgs.hpp"
# include "./Channel.hpp"
# include "./Commands.hpp"
# include "./Macros.hpp"

#endif