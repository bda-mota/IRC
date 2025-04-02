#include "../../includes/irc.hpp"

std::string CommandsArgs::topic(const std::vector<std::string>& args, Server& server, User* user) {

	if (args.empty()) {
		send(user->getFd(), "Error: No channel name provided.\r\n", 32, 0);
		return "ERROR: No channel name provided!\r\n";
	}

	const std::string& channelName = args[0];
	std::string newTopic = (args.size() > 1) ? args[1] : "";

	std::map<std::string, Channel>& channels = server.getChannels();
	
	if (channels.find(channelName) == channels.end()) {
		send(user->getFd(), "Error: channel not found.\r\n", 27, 0);
		return "ERROR: Channel not found!\r\n";
	}

	Channel& channel = channels[channelName];

	if (newTopic.empty()) {
		std::string response = ":" + user->getNickName() + " TOPIC " + channelName + " :" + channel.getTopic() + END;
		send(user->getFd(), response.c_str(), response.length(), 0);
		return "TOPIC command executed!\r\n";
	}

	channel.setTopic(newTopic);

	std::vector<User*>& users = channel.getUsers();
	for (std::vector<User*>::iterator it = users.begin(); it != users.end(); ++it) {
		User* currentUser = *it;
		std::string notify = ":" + user->getNickName() + " TOPIC " + channelName + " :" + newTopic + END;
		send(currentUser->getFd(), notify.c_str(), notify.length(), 0);
	}
	return "TOPIC command executed!\r\n";
}