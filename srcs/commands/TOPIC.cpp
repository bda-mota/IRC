#include "../../includes/irc.hpp"

std::string CommandsArgs::topic(const std::vector<std::string>& args, Server& server, User* user) {

	if (args.empty()) {
		send(user->getFd(), "Error: No channel name provided.\r\n", 32, 0);
		return "ERROR: No channel name provided!\r\n";
	}

	const std::string& channelName = args[0];

	if (channelName.empty() || channelName[0] != '#') {
		send(user->getFd(), "Error: Invalid channel name.\r\n", 31, 0);
		return "ERROR: Invalid channel name!\r\n";
	}

	std::string newTopic;

	for (size_t i = 1; i < args.size(); ++i) {
		newTopic += args[i];
		if (i != args.size() - 1) {
			newTopic += " ";
		}
	}

	std::map<std::string, Channel*>& channels = server.getChannels();
	
	if (channels.find(channelName) == channels.end()) {
		send(user->getFd(), "Error: channel not found.\r\n", 27, 0);
		return "ERROR: Channel not found!\r\n";
	}

	Channel* channel = channels[channelName];

	if (newTopic.empty()) {
		if (channel->getTopic().empty()) {
			std::string response = ":ircserver 331 " + user->getNickName() + " " + channelName + " :No topic is set" + END;
			send(user->getFd(), response.c_str(), response.length(), 0);
		} else {
			std::string response = ":ircserver 332 " + user->getNickName() + " " + channelName + " :" + channel->getTopic() + END;
			send(user->getFd(), response.c_str(), response.length(), 0);
		}
		return "TOPIC command executed!\r\n";
	}

	if (!channel->isUserInChannel(user)) {
		send(user->getFd(), "Error: You are not in the channel.\r\n", 36, 0);
		return "ERROR: You are not in the channel!\r\n";
	}

	if (!channel->isOperator(user)) {
		std::string error = ":ircserver 482 " + user->getNickName() + " " + channelName + " :You're not channel operator" + END;
		send(user->getFd(), error.c_str(), error.length(), 0);
		return "ERROR: You are not a channel operator!\r\n";
	}

	channel->setTopic(newTopic);

	std::vector<User*>& users = channel->getUsers();
	for (std::vector<User*>::iterator it = users.begin(); it != users.end(); ++it) {
		User* currentUser = *it;
		std::string notify = ":" + user->getNickName() + " TOPIC " + channelName + " :" + newTopic + END;
		send(currentUser->getFd(), notify.c_str(), notify.length(), 0);
	}
	return "TOPIC command executed!\r\n";
	//channels[channelName].broadcast(response, user);
}