#ifndef COMMANDS_HPP
# define COMMANDS_HPP

# include "./irc.hpp"

class User;
class Server;

// JOIN
void	join(Server& server, User* user, const std::string& channelName);

#endif