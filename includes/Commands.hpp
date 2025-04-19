#ifndef COMMANDS_HPP
# define COMMANDS_HPP

# include "./irc.hpp"

class User;
class Server;

// AUXILIARIES

bool	isValidChannelName(const std::string& channelName, User* user);
bool	channelExists(const std::string& channelName, Server& server, User* user);
bool	isUserInChannel(User& user, Channel& channel);
bool	isNickInUse(const std::string& nick, const std::vector<User*>& users);
void	sendWelcomeMessage(User* user);
void	sendError(User* user, const std::string& errorMsg);
void	sendResponse(User* user, const std::string& responseMsg);
Channel* findChannelInServer(Server& server, User* sender, const std::string& channelName);
User* findUserInServer(Server& server, User* sender, const std::string& targetNick);

#endif