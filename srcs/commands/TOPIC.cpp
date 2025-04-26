#include "../../includes/irc.hpp"

static void sendCurrentTopic(Channel* channel, User* user);
static void	defineNewTopic(const std::vector<std::string>& args, Channel* channel, User* user);

std::string CommandsArgs::topic(const std::vector<std::string>& args, Server& server, User* user) {
	if (args.empty()) {
		sendErrorAndLog(user, ERR_NOSUCHCHANNEL("*"));
		return "";
	}

	const std::string& channelName = args[0];
	if (!isValidChannelName(channelName, user) || !channelExists(channelName, server, user)) {
		sendErrorAndLog(user, ERR_NOSUCHCHANNEL(channelName));
		return "";
	}

	std::map<std::string, Channel*>& channels = server.getChannels();
	Channel* channel = channels[channelName];

	if (!channel->isUserInChannel(user)) {
		sendErrorAndLog(user, ERR_NOTONCHANNEL(channel->getName()));
		return "";
	}

	if (args.size() == 1) {
		sendCurrentTopic(channel, user);
		return "";
	}

	if (!channel->isOperator(user) && channel->isTopicRestricted() == true) {
		sendError(user, ERR_CHANOPRISNEEDED(user->getNickName(), channelName));
		return "";
	}

	defineNewTopic(args, channel, user);
	std::string notify = RPL_TOPIC(user->getNickName(), channelName, channel->getTopic());
	channel->broadcast(notify, user);

	logger(INFO, user->getNickName() + " changed topic for channel " + channel->getName() + " to: " + channel->getTopic());

	return "";
}

static void sendCurrentTopic(Channel* channel, User* user) {

	if (channel->getTopic().empty()) {
		std::string response = RPL_NOTOPIC(user->getNickName(), channel->getName());
		sendResponse(user, response);
		logger(INFO, user->getNickName() + " requested topic for channel " + channel->getName() + " but it is empty.");
	} else {
		std::string response = RPL_TOPIC(user->getNickName(), channel->getName(), channel->getTopic());
		sendResponse(user, response);
		logger(INFO, user->getNickName() + " requested topic for channel " + channel->getName() + ": " + channel->getTopic());
	}
}

static void	defineNewTopic(const std::vector<std::string>& args, Channel* channel, User* user) {
	std::string newTopic;
	for (size_t i = 1; i < args.size(); ++i) {
		newTopic += args[i];
		if (i != args.size() - 1) {
			newTopic += " ";
		}
	}
	channel->setTopic(newTopic, user);
}
