#ifndef IRC_HPP
# define IRC_HPP

// libraries
# include <iostream>
# include <string>
# include <vector>
# include <map>
# include <algorithm>
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
# include "./Channel.hpp"
# include "./Commands.hpp"
# include "./Macros.hpp"

#endif