#include "../../includes/irc.hpp"

void	join(Server& server, User* user, const std::string& channelName) {
	std::map<std::string, Channel>& channels = server.getChannels();

	if (channels.find(channelName) == channels.end()) {
		channels.insert(std::make_pair(channelName, Channel(channelName)));
	}

	channels[channelName].addUser(*user);

	std::string response = ":" + user->getNickName() + " JOIN " + channelName + END;
	send(user->getFd(), response.c_str(), response.length(), 0);

	std::vector<User*>& users = channels[channelName].getUsers();
	for (std::vector<User*>::iterator it = users.begin(); it != users.end(); ++it) {
		User* currentUser = *it;
		if (currentUser->getFd() != user->getFd()) {
			std::string notify = ":" + user->getNickName() + " entrou no canal " + channelName + END;
			send(currentUser->getFd(), notify.c_str(), notify.length(), 0);
		}
	}
	//channels[channelName].broadcast(response, user);
}