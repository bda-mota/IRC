#include "../../includes/irc.hpp"

void	topic(Server& server, User* user, const std::string& channelName, const std::string& newTopic) {
	std::map<std::string, Channel>& channels = server.getChannels();
	
	if (channels.find(channelName) == channels.end()) {
		send(user->getFd(), "Error: channel not found.\r\n", 27, 0);
		return ;
	}

	Channel& channel = channels[channelName];

	if (newTopic.empty()) {
		std::string response = ":" + user->getNickName() + " TOPIC " + channelName + " :" + channel.getTopic() + END;
		send(user->getFd(), response.c_str(), response.length(), 0);
		return ;
	}

	channel.setTopic(newTopic);

	std::vector<User*>& users = channel.getUsers();
	for (std::vector<User*>::iterator it = users.begin(); it != users.end(); ++it) {
		User* currentUser = *it;
		std::string notify = ":" + user->getNickName() + " TOPIC " + channelName + " :" + newTopic + END;
		send(currentUser->getFd(), notify.c_str(), notify.length(), 0);
	}
}