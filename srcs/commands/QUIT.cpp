#include "../../includes/irc.hpp"

std::string CommandsArgs::quit(const std::vector<std::string>& args, Server& server, User* user) {
	std::string reason;

	if (!args.empty()) {
		for (size_t i = 0; i < args.size(); i++) {
			if (i == 0)
				reason = args[i];
			else
				reason += " " + args[i];
		}
	} else {
		reason = "Client Quit";
	}

	std::ostringstream quitMsg;
	quitMsg << ":" << user->getNickName() << "!" << user->getUserName()
	        << "@" << user->getHostName() << " QUIT :" << reason << "\r\n";

	std::vector<Channel*> joinedChannels = user->getJoinedChannels();

	for (size_t i = 0; i < joinedChannels.size(); ++i) {
		Channel* channel = joinedChannels[i];
		channel->sendToAll(quitMsg.str());
		channel->removeUser(user->getFd());

		if (channel->getUsers().empty()) {
			server.getChannels().erase(channel->getName());
			delete channel;
		}
	}

	server.clearUsers(user->getFd());
	close(user->getFd());

	return "";
}
