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

	std::string channelName = args[0];
	if (!isValidChannelName(channelName, user)) {
		sendErrorAndLog(user, ERR_NOSUCHCHANNEL(channelName));
		return "";
	}

	createChannelIfNotExists(channelName, server, user);
	Channel* channel = server.getChannels()[channelName];
	createChannelIfNotExists(channelName, server, user);

    // Verifica se canal tem senha e se ela foi fornecida corretamente
    if (channel->hasKey() && (args.size() < 2 || args[1] != channel->getChannelKey())) {
        sendErrorAndLog(user, ERR_BADCHANNELKEY(user->getNickName(), channelName));
        return "";
    }

	// verifica a permissão do canal como olny invite e se o user tem um convite
	if (!canUserJoinChannel(channel, user, channelName)) {
		sendErrorAndLog(user, ERR_INVITEONLYCHAN(channelName));
		return "";
	}

    // Verifica se canal está cheio
    if (channel->isFull()) {
        sendErrorAndLog(user, ERR_CHANNELISFULL(channelName));
        return "";
    }

	addUserToChannel(channel, user);
	user->removeInvitation(channelName); // remove a solicitação da lista de convites depois que o user entra no canal
	sendListOfUsers(channel, user);

	std::string notify = JOIN(user->getNickName(), channelName);
	channel->broadcast(notify, user);

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