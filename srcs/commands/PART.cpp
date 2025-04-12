#include "../../includes/irc.hpp"

/*
    PART <canal> [:mensagem de saída opcional]
    PART #programadoras :tô vazando
*/

static void auxBuildReason(std::string& reason, const std::vector<std::string>& args, size_t start);

std::string CommandsArgs::part(const std::vector<std::string>& args, Server& server, User* user) {

	if (args.empty()) {
		std::string error = ":ircserver 461 " + user->getNickName() + " PART :Not enough parameters\r\n";
		send(user->getFd(), error.c_str(), error.length(), 0);
		return "";
	}

	std::string channelName = args[0];
	std::string reason;

    if (args.size() > 1)
	    auxBuildReason(reason, args, 1);
    else
	    reason = "Leaving";

    std::map<std::string, Channel*>& channels = server.getChannels();

	if (channels.find(channelName) == channels.end()) {
		std::string error = ":ircserver 403 " + user->getNickName() + " " + channelName + " :No such channel\r\n";
		send(user->getFd(), error.c_str(), error.length(), 0);
		return "";
	}

	Channel* channel = channels[channelName];

    if (!channel->isUserInChannel(user)) {
		std::string error = ":ircserver 442 " + user->getNickName() + " " + channelName + " :You're not on that channel\r\n";
		send(user->getFd(), error.c_str(), error.length(), 0);
		return "";
	}

    channel->removeUser(user->getFd());
    user->removeChannel(channel);

    std::ostringstream partMsg;
    partMsg << ":" << user->getNickName() << "!" << user->getUserName()
        << "@" << user->getHostName() << " PART " << channelName
        << " :" << reason << "\r\n";

    channel->sendToAll(partMsg.str());

    if (channel->getUsers().empty()) { 
	    delete channel;
	    channels.erase(channelName); 
    }

	return "";
}

static void auxBuildReason(std::string& reason, const std::vector<std::string>& args, size_t start) {
	for (size_t i = start; i < args.size(); i++) {
		if (i == start)
			reason = args[i].substr(0);
		else
			reason += " " + args[i];
	}
}