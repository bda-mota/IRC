#ifndef COMMANDS_HPP
# define COMMANDS_HPP

# include "./irc.hpp"

class User;
class Server;

// COMMANDS
void	join(Server& server, User* user, const std::string& channelName);
void	privmsg(Server& server, User* sender, const std::string& target, const std::string& message);

// AUXILIARIES
bool	isUserInChannel(User& user, Channel& channel);

#endif