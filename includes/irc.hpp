#ifndef IRC_HPP
# define IRC_HPP

// libraries
# include <iostream>
# include <string>
# include <vector>
# include <map>
# include <set>
# include <sstream>
# include <algorithm>
# include <cstring>
# include <unistd.h>
# include <sys/socket.h> 
# include <netinet/in.h>
# include <poll.h>  
# include <fcntl.h>
# include <signal.h>
# include <stdexcept>
# include <arpa/inet.h>
# include <ctime>
# include <iomanip>

// files includes 
# include "./User.hpp"
# include "./Server.hpp"
# include "./Command.hpp"
# include "./CommandsArgs.hpp"
# include "./Channel.hpp"
# include "./Commands.hpp"
# include "./Macros.hpp"

#endif