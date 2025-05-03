#include "../../includes/irc.hpp"

static void auxBuildReason(std::string& reason, const std::vector<std::string>& args, size_t start);
static void removeUserFromChannel(User* user, Channel* channel);

std::string CommandsArgs::part(const std::vector<std::string>& args, Server& server, User* user) {
	if (args.empty()) {
		sendErrorAndLog(user, ERR_NEEDMOREPARAMS("PART", "Not enough parameters"));
		return "";
	}

	std::string reason;
	if (args.size() > 1)
		auxBuildReason(reason, args, 1);
	else
		reason = "Leaving";

	std::vector<std::string> channelNames;
	std::stringstream ss(args[0]);
	std::string channelName;
	while (std::getline(ss, channelName, ',')) {
		if (!channelName.empty())
			channelNames.push_back(channelName);
	}

	std::map<std::string, Channel*>& channels = server.getChannels();

	for (size_t i = 0; i < channelNames.size(); ++i) {
		const std::string& name = channelNames[i];

		if (channels.find(name) == channels.end()) {
			sendErrorAndLog(user, ERR_NOSUCHCHANNEL(name));
			continue;
		}

		Channel* channel = channels[name];

		if (!channel->isUserInChannel(user)) {
			sendErrorAndLog(user, ERR_USERNOTINCHANNEL(user->getNickName(), user->getNickName(), name));
			continue;
		}

		removeUserFromChannel(user, channel);

		std::string partMsg = RPL_PARTMSG(user->getNickName(), user->getUserName(), name, reason);
		channel->broadcast(partMsg, user);
		send(user->getFd(), partMsg.c_str(), partMsg.length(), 0);

		if (channel->getUsers().empty()) {
			delete channel;
			channels.erase(name);
			logger(INFO, user->getNickName() + " left channel " + name + " with reason: " + reason);
			return "";
		}

		promoteOperatorChannel(user, channel, "PART");
		logger(INFO, user->getNickName() + " left channel " + name + " with reason: " + reason);
	}
	return "";
}

static void auxBuildReason(std::string& reason, const std::vector<std::string>& args, size_t start) {
	reason.clear();
	for (size_t i = start; i < args.size(); i++) {
		if (i != start)
			reason += " ";
		reason += args[i];
	}
}

static void removeUserFromChannel(User* user, Channel* channel) {
	channel->removeUser(user);
	user->removeChannel(channel);
}
