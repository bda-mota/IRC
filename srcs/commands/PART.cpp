#include "../../includes/irc.hpp"

static void auxBuildReason(std::string& reason, const std::vector<std::string>& args, size_t start);

std::string CommandsArgs::part(const std::vector<std::string>& args, Server& server, User* user) {

	if (args.empty()) {
		sendError(user, ERR_NEEDMOREPARAMS("PART", "Not enough parameters"));
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
		sendError(user, ERR_NOSUCHCHANNEL(channelName));
		return "";
	}

	Channel* channel = channels[channelName];

    if (!channel->isUserInChannel(user)) {
		sendError(user, ERR_USERNOTINCHANNEL(user->getNickName(), user->getNickName(), channelName));
		return "";
	}

    channel->removeUser(user->getFd());
    user->removeChannel(channel);

	std::string partMsg = RPL_PARTMSG(user->getNickName(), user->getUserName(), channelName, reason);
	channel->broadcast(partMsg, user);


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