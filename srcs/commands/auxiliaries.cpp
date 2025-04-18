#include "../../includes/irc.hpp"

// CHANNEL

bool	isValidChannelName(const std::string& channelName, User* user) {
	if (channelName.empty() || channelName[0] != '#') {
		std::string error = ERR_NOSUCHCHANNEL(channelName);
		sendError(user, error);
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
		sendError(user, error);
		return false;
	}
	return true;
}
// USER

bool	isNickInUse(const std::string& nick, const std::vector<User*>& users) {
	for (std::vector<User*>::const_iterator it = users.begin(); it != users.end(); ++it) {
		if ((*it)->getNickName() == nick) {
			return true;
		}
	}
	return false;
}

// SEND

void	sendWelcomeMessage(User* user) {
	if (!user->getRegistered() && user->getHasNickCommand() && user->getHasUserCommand()) { // add a verificacao do pass
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