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

bool	isNickInUse(const std::string& nick, const std::vector<User>& users) {
	for (std::vector<User>::const_iterator it = users.begin(); it != users.end(); ++it) {
		if (it->getNickName() == nick) {
			return true;
		}
	}
	return false;
}