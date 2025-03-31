#include "../../includes/irc.hpp"

bool isUserInChannel(User& user, Channel& channel) {
	std::vector<User*>& users = channel.getUsers();
	for (std::vector<User*>::iterator it = users.begin(); it != users.end(); ++it) {
		if ((*it)->getFd() == user.getFd()) {
			return true;
		}
	}
	return false;
}