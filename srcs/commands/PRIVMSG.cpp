#include "../../includes/irc.hpp"

std::string CommandsArgs::privmsg(const std::vector<std::string>& args, Server& server, User* sender) {
	
	if (args.size() < 2) {
		std::string error = ":ircserver 412 " + sender->getNickName() + " :No text to send" + END;
		send(sender->getFd(), error.c_str(), error.length(), 0);
		return "ERROR PRIVMSG\r\n";
	}

	const std::string& target = args[0];
	const std::string& message = args[1];

	// if is a channel 
	if (target[0] == '#') {
		std::map<std::string, Channel>& channels = server.getChannels();
		if (channels.find(target) == channels.end()) {
			std::string error = ":ircserver 403 " + sender->getNickName() + " " + target + " :No such channel" + END;
			send(sender->getFd(), error.c_str(), error.length(), 0);
			return "PRIVMSG command executed!\r\n";
		}

		Channel& channel = channels[target];
		if (!isUserInChannel(*sender, channel)) {
			std::string error = ":ircserver 404 " + sender->getNickName() + " " + target + " :Cannot send to channel" + END;
			send(sender->getFd(), error.c_str(), error.length(), 0);
			return "PRIVMSG command executed!\r\n";
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
			return "PRIVMSG command executed!\r\n";
		}
	}
	
	std::string error = ":ircserver 401 " + sender->getNickName() + " " + target + " :No such nick/channel" + END;
	send(sender->getFd(), error.c_str(), error.length(), 0);

	return "PRIVMSG command executed!\r\n";
}
