#include "../../includes/irc.hpp"

static void	createChannelIfNotExists(const std::string& channelName, Server& server, User* user);
static void	addUserToChannel(Channel* channel, User* user);
static void	sendListOfUsers(Channel *channel, User* user);
static bool canUserJoinChannel(Channel* channel, User* user, const std::string& channelName);

std::string CommandsArgs::join(const std::vector<std::string>& args, Server& server, User* user) {

	if (args.empty()) {
		sendErrorAndLog(user, ERR_NEEDMOREPARAMS("JOIN", ""));
		return "";
	}

	std::vector<std::string> channelNames;
	std::stringstream ss(args[0]);
	std::string channelName;
	while (std::getline(ss, channelName, ',')) {
		if (!channelName.empty())
			channelNames.push_back(channelName);
	}

	std::vector<std::string> keys;
	if (args.size() > 1) {
		std::stringstream ss_keys(args[1]);
		std::string key;
		while (std::getline(ss_keys, key, ',')) {
			keys.push_back(key);
		}
	}

	std::map<std::string, Channel*>& channels = server.getChannels();

	for (size_t i = 0; i < channelNames.size(); ++i) {
		const std::string& channelName = channelNames[i];

		if (!isValidChannelName(channelName, user)) {
			sendErrorAndLog(user, ERR_NOSUCHCHANNEL(channelName));
			continue;
		}

		createChannelIfNotExists(channelName, server, user);
		Channel* channel = channels[channelName];

		if (channel->hasKey()) {
			if (i >= keys.size() || keys[i] != channel->getChannelKey()) {
				sendErrorAndLog(user, ERR_BADCHANNELKEY(user->getNickName(), channelName));
				continue;
			}
		}

		if (!canUserJoinChannel(channel, user, channelName)) {
			sendErrorAndLog(user, ERR_INVITEONLYCHAN(channelName));
			continue;
		}

		if (channel->isFull()) {
			sendErrorAndLog(user, ERR_CHANNELISFULL(channelName));
			continue;
		}

		addUserToChannel(channel, user);
		user->removeInvitation(channelName);
		sendListOfUsers(channel, user);
	}

	return "";
}

static void	createChannelIfNotExists(const std::string& channelName, Server& server, User* user) {
	std::map<std::string, Channel*>& channels = server.getChannels();

	if (channels.find(channelName) == channels.end()) {
		channels[channelName] = new Channel(channelName);
		channels[channelName]->addOperator(user);
	}
}

static void	addUserToChannel(Channel* channel, User* user) {
	if (channel->isUserInChannel(user))
    	return;

	channel->addUser(user);
	user->joinChannel(channel);

	std::string response = JOIN(user->getNickName(), channel->getName());
	sendResponse(user, response);
	logger(INFO, user->getNickName() + " joined channel " + channel->getName());
}

static void	sendListOfUsers(Channel *channel, User* user) {
	std::vector<User*>& users = channel->getUsers();
	std::string names;
	for (std::vector<User*>::iterator it = users.begin(); it != users.end(); ++it) {
		names += (*it)->getNickName() + " ";
	}
	if (!names.empty())
		names.erase(names.length() - 1);

    logger(INFO, user->getNickName() + " requested user list for channel " + channel->getName());
    logger(INFO, "User list for channel " + channel->getName() + ": " + names);
    
	std::string nameReply = RPL_NAMREPLY(user->getNickName(), channel->getName(), names);
	sendResponse(user, nameReply);

	std::string endOfNames = RPL_ENDOFNAMES(user->getNickName(), channel->getName());
	sendResponse(user, endOfNames);
}

static bool canUserJoinChannel(Channel* channel, User* user, const std::string& channelName) {
	if (channel->isInviteOnly() && !user->isInvitedTo(channelName)) {
		return false;
	}
	return true;
}
