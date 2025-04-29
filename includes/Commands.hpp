#ifndef COMMANDS_HPP
# define COMMANDS_HPP

# include "./irc.hpp"

class User;
class Server;

// LogLevel enum to categorize log messages
enum LogLevel {
    INFO,
    WARNING,
    ERROR
};

// AUXILIARIES

bool	 isValidChannelName(const std::string& channelName, User* user);
bool	 channelExists(const std::string& channelName, Server& server, User* user);
bool	 isUserInChannel(User& user, Channel& channel);
bool	 isNickInUse(const std::string& nick, const std::vector<User*>& users);
void	 sendWelcomeMessage(User* user);
void	 sendError(User* user, const std::string& errorMsg);
void	 sendResponse(User* user, const std::string& responseMsg);
Channel* findChannelInServer(Server& server, User* sender, const std::string& channelName);
User*    findUserInServer(Server& server, User* sender, const std::string& targetNick);
void     buildTrailingMessage(std::string& message, const std::vector<std::string>& args, size_t start);
void     sendErrorAndLog(User* user, const std::string& errorMessage);
void     logger(LogLevel level, const std::string& message);

// AUXILIARIES MODE COMMAND

void	inviteOnlyConfig(Channel* channel, char sign);
void	topicCmdConfig(Channel* channel, char modeSign);
std::string userLimitConfig(Channel* channel, char modeSign, const std::string& extraArg);
void	channelKeyConfig(Channel* channel, char modeSign, const std::string& extraArg);
void	channelOpConfig(Channel* channel, char modeSign, const std::string& extraArg);

#endif