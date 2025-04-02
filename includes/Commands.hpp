#ifndef COMMANDS_HPP
# define COMMANDS_HPP

# include "./irc.hpp"

class User;
class Server;

// AUXILIARIES
bool	isUserInChannel(User& user, Channel& channel);

#endif