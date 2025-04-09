#include "../../includes/irc.hpp"

std::string CommandsArgs::privmsg(const std::vector<std::string>& args, Server& server, User* sender) {
	if (args.size() < 2) {
		std::string error = ":ircserver 412 " + sender->getNickName() + " :No text to send" + END;
		send(sender->getFd(), error.c_str(), error.length(), 0);
		return "ERROR PRIVMSG\r\n";
	}

	const std::string& target = args[0];

	if (args[1][0] != ':') {
		std::string error = ":ircserver 412 " + sender->getNickName() + " :No text to send" + END;
		send(sender->getFd(), error.c_str(), error.length(), 0);
		return "ERROR: No text to send\r\n";
	}

	std::string message;
	for (size_t i = 1; i < args.size(); ++i) {
		message += args[i];
		if (i != args.size() - 1)
			message += " ";
	}
	if (!message.empty() && message[0] == ':')
		message = message.substr(1);

	std::cout << "Message: " << message << std::endl;

	// Mensagem para canal
	if (target[0] == '#') {
		std::map<std::string, Channel*>& channels = server.getChannels();
		if (channels.find(target) == channels.end()) {
			std::string error = ":ircserver 403 " + sender->getNickName() + " " + target + " :No such channel" + END;
			send(sender->getFd(), error.c_str(), error.length(), 0);
			return "PRIVMSG command executed!\r\n";
		}

		Channel* channel = channels[target];
		if (!isUserInChannel(*sender, *channel)) {
			std::string error = ":ircserver 404 " + sender->getNickName() + " " + target + " :Cannot send to channel" + END;
			send(sender->getFd(), error.c_str(), error.length(), 0);
			return "PRIVMSG command executed!\r\n";
		}

		std::string response = ":" + sender->getNickName() + " PRIVMSG " + target + " :" + message + END;
		std::vector<User*>& users = channel->getUsers();
		for (std::vector<User*>::iterator it = users.begin(); it != users.end(); ++it) {
			User* currentUser = *it;
			if (currentUser->getFd() != sender->getFd()) {
				send(currentUser->getFd(), response.c_str(), response.length(), 0);
			}
		}
		return "PRIVMSG on channel command executed!\r\n";
	}

	std::vector<User*>& users = server.getUsers();
	for (std::vector<User*>::iterator it = users.begin(); it != users.end(); ++it) {
		if ((*it)->getNickName() == target) {
			std::string response = ":" + sender->getNickName() + " PRIVMSG " + target + " :" + message + END;
			send((*it)->getFd(), response.c_str(), response.length(), 0);
			return "PRIVMSG to user command executed!\r\n";
			//tentar usar o broadcast
		}
	}

	std::string error = ":ircserver 401 " + sender->getNickName() + " " + target + " :No such nick/channel" + END;
	send(sender->getFd(), error.c_str(), error.length(), 0);
	return "PRIVMSG command executed!\r\n";
}
