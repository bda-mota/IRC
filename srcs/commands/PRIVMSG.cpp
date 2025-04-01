#include "../../includes/irc.hpp"

void	privmsg(Server& server, User* sender, const std::string& target, const std::string& message) {
	// check if the message is empty
	if (message.empty()) {
		std::string error = ":ircserver 412 " + sender->getNickName() + " :No text to send" + END;
		send(sender->getFd(), error.c_str(), error.length(), 0);
		return;
	}

	// if is a channel 
	if (target[0] == '#') {
		std::map<std::string, Channel>& channels = server.getChannels();
		if (channels.find(target) == channels.end()) {
			std::string error = ":ircserver 403 " + sender->getNickName() + " " + target + " :No such channel" + END;
			send(sender->getFd(), error.c_str(), error.length(), 0);
			return;
		}

		Channel& channel = channels[target];
		if (!isUserInChannel(*sender, channel)) {
			std::string error = ":ircserver 404 " + sender->getNickName() + " " + target + " :Cannot send to channel" + END;
			send(sender->getFd(), error.c_str(), error.length(), 0);
			return;
		}

		std::string response = ":" + sender->getNickName() + " PRIVMSG " + target + " :" + message + END;
		std::vector<User*>& users = channel.getUsers();
		for (std::vector<User*>::iterator it = users.begin(); it != users.end(); ++it) {
			User* currentUser = *it;
			if (currentUser->getFd() != sender->getFd()) {
				send(currentUser->getFd(), response.c_str(), response.length(), 0);
			}
		}
	}

	// if is a user 
	std::vector<User>& users = server.getUsers();
	for (std::vector<User>::iterator it = users.begin(); it != users.end(); ++it) {
		if (it->getNickName() == target) {
			std::string response = ":" + sender->getNickName() + " PRIVMSG " + target + " :" + message + END;
			send(it->getFd(), response.c_str(), response.length(), 0);
			return;
		}
	}
	
	std::string error = ":ircserver 401 " + sender->getNickName() + " " + target + " :No such nick/channel" + END;
	send(sender->getFd(), error.c_str(), error.length(), 0);
}
