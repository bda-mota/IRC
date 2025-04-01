#ifndef COMMANDS_HPP
# define COMMANDS_HPP

# include "./irc.hpp"

class User;
class Server;

// COMMANDS
void	join(Server& server, User* user, const std::string& channelName);
void	privmsg(Server& server, User* sender, const std::string& target, const std::string& message);
void	topic(Server& server, User* user, const std::string& channelName, const std::string& newTopic);
void	who(Server& server, User* user, const std::string& target);
void	nick(Server& server, User* user, const std::string& newNick);


// AUXILIARIES
bool	isUserInChannel(User& user, Channel& channel);
bool	isNickInUse(const std::string& nick, const std::vector<User>& users);

#endif