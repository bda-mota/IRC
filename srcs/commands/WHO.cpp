#include "../../includes/irc.hpp"

std::string CommandsArgs::who(const std::vector<std::string>& args, Server& server, User* user) {
	std::vector<User*> targetUsers;
	
	if (args.empty()) {
		std::map<std::string, Channel*>& channels = server.getChannels();
		for (std::map<std::string, Channel*>::iterator it = channels.begin(); it != channels.end(); ++it) {
			Channel* channel = it->second;
			if (!channel->isUserInChannel(user))
				continue;

			std::vector<User*>& users = channel->getUsers();
			for (std::vector<User*>::iterator uit = users.begin(); uit != users.end(); ++uit) {
				User* u = *uit;
				if (!u) continue;
				std::string response = ":ircserver 352 " + user->getNickName() + " " + channel->getName() + " " + u->getUserName()
					+ " " + u->getIP() + " ircserver " + u->getNickName() + " H :0 " + u->getRealName() + END;
				send(user->getFd(), response.c_str(), response.length(), 0);
			}
		}
		std::string endResponse = ":ircserver 315 " + user->getNickName() + " * :End of WHO list." + END;
		send(user->getFd(), endResponse.c_str(), endResponse.length(), 0);
		return "";
	}

	std::string target = args[0];
	if (target[0] == '#') {
		std::map<std::string, Channel*>& channels = server.getChannels();
		if (channels.find(target) == channels.end()) {
			send(user->getFd(), "Error: channel not found.\r\n", 27, 0);
			return "";
		}
		targetUsers = channels[target]->getUsers();
	} else {
		std::vector<User*>& allUsers = server.getUsers();
		for (std::vector<User*>::iterator it = allUsers.begin(); it != allUsers.end(); ++it) {
			if ((*it)->getNickName().find(target) != std::string::npos) {
				targetUsers.push_back((*it));
			}
		}
	}

	for (std::vector<User*>::iterator it = targetUsers.begin(); it != targetUsers.end(); it++) {
		User *u = *it;
		std::string response = ":ircserver 352 " + user->getNickName() + " " + target + " " + u->getUserName()
			+ " " + u->getIP() + " ircserver " + u->getNickName() + " H :0 " + u->getRealName() + END;
		send(user->getFd(), response.c_str(), response.length(), 0);
	}

	std::string endResponse = ":ircserver 315 " + user->getNickName() + " " + target + " :End of WHO list." + END;
	send(user->getFd(), endResponse.c_str(), endResponse.length(), 0);
	return "";
}