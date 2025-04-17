#include "../../includes/irc.hpp"

bool	isUserInChannel(User& user, Channel& channel) {
	std::vector<User*>& users = channel.getUsers();
	for (std::vector<User*>::iterator it = users.begin(); it != users.end(); ++it) {
		if ((*it)->getFd() == user.getFd()) {
			return true;
		}
	}
	return false;
}

bool	isNickInUse(const std::string& nick, const std::vector<User*>& users) {
	for (std::vector<User*>::const_iterator it = users.begin(); it != users.end(); ++it) {
		if ((*it)->getNickName() == nick) {
			return true;
		}
	}
	return false;
}

void	sendWelcomeMessage(User* user) {
	if (!user->getRegistered() && user->getHasNickCommand() && user->getHasUserCommand()) { // add a verificacao do pass
		std::string welcome = RPL_WELCOME(user->getNickName(), user->getUserName());
		send(user->getFd(), welcome.c_str(), welcome.length(), 0);
		user->setRegistered(true);
	}
}