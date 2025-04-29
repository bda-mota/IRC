#include "../../includes/irc.hpp"

static void sendWhoReply(User* requester, const std::string& channel, User* target);
static void sendChannelWhoReplies(User* requester, Channel* channel);
static void sendAllChannelsWho(Server& server, User* user);
static void handleChannelWho(const std::string& channelName, Server& server, User* user);
static void handleUserWho(const std::string& target, Server& server, User* user);

std::string CommandsArgs::who(const std::vector<std::string>& args, Server& server, User* user) {
	if (args.empty()) {
		sendAllChannelsWho(server, user);
		return "";
	}

	std::string target = args[0];
	if (target[0] == '#') {
		handleChannelWho(target, server, user);
	} else {
		handleUserWho(target, server, user);
	}

	return "";
}

static void sendWhoReply(User* requester, const std::string& channel, User* target) {
	std::string response = RPL_WHOREPLY(
		channel,
		target->getUserName(),
		target->getNickName(),
		"H",
		target->getRealName()
	);
	sendResponse(requester, response);
}

static void sendChannelWhoReplies(User* requester, Channel* channel) {
	if (!channel->isUserInChannel(requester))
		return;

	const std::vector<User*>& users = channel->getUsers();
	for (size_t i = 0; i < users.size(); ++i) {
		if (users[i])
			sendWhoReply(requester, channel->getName(), users[i]);
	}
}

static void sendAllChannelsWho(Server& server, User* user) {
	std::map<std::string, Channel*>& channels = server.getChannels();
	for (std::map<std::string, Channel*>::iterator it = channels.begin(); it != channels.end(); ++it) {
		sendChannelWhoReplies(user, it->second);
	}
	std::string response = RPL_ENDOFWHO(user->getNickName());
	sendResponse(user, response);
	logger(INFO, user->getNickName() + " requested WHO for all channels.");
}

static void handleChannelWho(const std::string& channelName, Server& server, User* user) {
	std::map<std::string, Channel*>& channels = server.getChannels();
	if (channels.find(channelName) == channels.end()) {
		sendErrorAndLog(user, ERR_NOSUCHCHANNEL(channelName));
		return;
	}

	std::vector<User*>& users = channels[channelName]->getUsers();
	for (size_t i = 0; i < users.size(); ++i) {
		if (users[i])
			sendWhoReply(user, channelName, users[i]);
	}
	std::string response = RPL_ENDOFWHO(channelName);
	sendResponse(user, response);
	logger(INFO, user->getNickName() + " requested WHO for channel " + channelName);
}

static void handleUserWho(const std::string& target, Server& server, User* user) {
	std::vector<User*>& allUsers = server.getUsers();
	for (size_t i = 0; i < allUsers.size(); ++i) {
		if (allUsers[i]->getNickName().find(target) != std::string::npos)
			sendWhoReply(user, target, allUsers[i]);
	}
	std::string response = RPL_ENDOFWHO(target);
	sendResponse(user, response);
	logger(INFO, user->getNickName() + " requested WHO for user " + target);
}