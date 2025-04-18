#include "../../includes/irc.hpp"

static void sendCurrentTopic(Channel* channel, User* user);
static void	defineNewTopic(const std::vector<std::string>& args, Channel* channel);

std::string CommandsArgs::topic(const std::vector<std::string>& args, Server& server, User* user) {
	if (args.empty()) {
		std::string error = ERR_NOSUCHCHANNEL("*");
		sendError(user, error);
		return "ERROR TOPIC\r\n";
	}

	const std::string& channelName = args[0];
	if (!isValidChannelName(channelName, user) || !channelExists(channelName, server, user)) {
		return "ERROR TOPIC\r\n";
	}
		
	std::map<std::string, Channel*>& channels = server.getChannels();
	Channel* channel = channels[channelName];

	if (!channel->isUserInChannel(user)) {
		std::string error = ERR_NOTONCHANNEL(channel->getName());
		sendError(user, error);
		return error;
	}
	
	if (args.size() == 1) {
		sendCurrentTopic(channel, user);
		return "TOPIC check complete" + CRLF;
	}
	
	if (!channel->isOperator(user)) {
		std::string error = ERR_CHANOPRISNEEDED(user->getNickName(), channelName);
		sendError(user, error);
		return error;
	}

	defineNewTopic(args, channel);
	std::string notify = RPL_TOPIC(user->getNickName(), channelName, channel->getTopic());
	channel->broadcast(notify, user);

	return "TOPIC updated" + CRLF;
}

static void sendCurrentTopic(Channel* channel, User* user) {

	if (channel->getTopic().empty()) {
		std::string response = RPL_NOTOPIC(user->getNickName(), channel->getName());
		sendResponse(user, response);
	} else {
		std::string response = RPL_TOPIC(user->getNickName(), channel->getName(), channel->getTopic());
		sendResponse(user, response);
	}
}

static void	defineNewTopic(const std::vector<std::string>& args, Channel* channel) {
	std::string newTopic;
	for (size_t i = 1; i < args.size(); ++i) {
		newTopic += args[i];
		if (i != args.size() - 1) {
			newTopic += " ";
		}
	}
	channel->setTopic(newTopic);
}