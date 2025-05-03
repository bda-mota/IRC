#include "../../includes/irc.hpp"

// Channel

bool	isValidChannelName(const std::string& channelName, User* user) {
  if (channelName.empty() || channelName[0] != '#') {
		std::string error = ERR_NOSUCHCHANNEL(channelName);
		sendErrorAndLog(user, error);
		return false;
	}
	return true;
}

bool	isUserInChannel(User& user, Channel& channel) {
	std::vector<User*>& users = channel.getUsers();
	for (std::vector<User*>::iterator it = users.begin(); it != users.end(); ++it) {
		if ((*it)->getFd() == user.getFd()) {
			return true;
		}
	}
	return false;
}

bool	channelExists(const std::string& channelName, Server& server, User* user) {
	if (!isValidChannelName(channelName, user)) {
		return false;
	}

	std::map<std::string, Channel*>& channels = server.getChannels();
	if (channels.find(channelName) == channels.end()) {
		std::string error = ERR_NOSUCHCHANNEL(channelName);
		sendErrorAndLog(user, error);
		return false;
	}
	return true;
}

void promoteOperatorChannel(User *user, Channel *channel, std::string mode) {
	if (channel->isOperator(user)) {
		if (mode != "QUIT")
			channel->removeOperator(user);
		std::vector<User*> users = channel->getUsers();
		if (channel->getOperators().empty()) {
			channel->addOperator(users[0]);
			channel->broadcastToAll( ":" + user->getNickName() + " MODE " + channel->getName() + " +o " + users[0]->getNickName() + CRLF);
			sendResponse(user, ":" + user->getNickName() + " MODE " + channel->getName() + " +o " + users[0]->getNickName() + CRLF);
		}
	}
}

Channel* findChannelInServer(Server& server, User* sender, const std::string& channelName) {
    std::map<std::string, Channel*>& channels = server.getChannels();

    if (channels.find(channelName) != channels.end()) {
        return channels[channelName];
    }

    sendErrorAndLog(sender, ERR_NOSUCHCHANNEL(channelName));
    return NULL;
}

// User

bool	isNickInUse(const std::string& nick, const std::vector<User*>& users) {
	for (std::vector<User*>::const_iterator it = users.begin(); it != users.end(); ++it) {
		if ((*it)->getNickName() == nick) {
			return true;
		}
	}
	return false;
}

User* findUserInServer(Server& server, User* sender, const std::string& targetNick) {
    std::vector<User*>& users = server.getUsers();

    for (size_t i = 0; i < users.size(); ++i) {
        if (users[i]->getNickName() == targetNick) {
            return users[i];
        }
    }

    sendErrorAndLog(sender, ERR_NOSUCHNICK(targetNick));
    return NULL;
}

// Send

void	sendWelcomeMessage(User* user) {
	if (!user->getRegistered() &&
		user->getHasNickCommand() &&
		user->getHasUserCommand() &&
		user->getHasPassCommand()) {
		std::string welcome = RPL_WELCOME(user->getNickName(), user->getUserName());
		send(user->getFd(), welcome.c_str(), welcome.length(), 0);
		user->setRegistered(true);
	}
}

void	sendError(User* user, const std::string& errorMsg) {
    send(user->getFd(), errorMsg.c_str(), errorMsg.length(), 0);
}

void	sendResponse(User* user, const std::string& responseMsg) {
    send(user->getFd(), responseMsg.c_str(), responseMsg.length(), 0);
}

void buildTrailingMessage(std::string& message, const std::vector<std::string>& args, size_t start) {
	std::ostringstream oss;
	oss << args[start].substr(1);
	for (size_t i = start + 1; i < args.size(); ++i)
		oss << ' ' << args[i];
	message = oss.str();
}


// Logger
void logger(LogLevel level, const std::string& message) {
    std::time_t now = std::time(NULL);
    std::tm *ltm = std::localtime(&now);

    std::cout << "[" << std::setfill('0') << std::setw(2) << ltm->tm_hour
              << ":" << std::setfill('0') << std::setw(2) << ltm->tm_min
              << ":" << std::setfill('0') << std::setw(2) << ltm->tm_sec
              << "] ";

    switch (level) {
        case INFO:
            std::cout << BLUE << "[INFO] " << RESET;
            break;
        case WARNING:
            std::cout << YELLOW << "[WARNING] " << RESET;
            break;
        case ERROR:
            std::cout << RED << "[ERROR] " << RESET;
            break;
    }

    std::cout << message << std::endl;
}

// Error
void sendErrorAndLog(User* user, const std::string& errorMessage) {
  sendError(user, errorMessage);
  std::string serverErrorMessage = errorMessage.substr(0, errorMessage.length() - 2);
	logger(ERROR, "Error sent to user " + user->getNickName() + ": " + serverErrorMessage);
}
