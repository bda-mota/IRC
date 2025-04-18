#include "../../includes/irc.hpp"

static void	createChannelIfNotExists(const std::string& channelName, Server& server, User* user);
static void	addUserToChannel(Channel* channel, User* user);
static void	sendListOfUsers(Channel *channel, User* user);
static bool canUserJoinChannel(Channel* channel, User* user, const std::string& channelName);

std::string CommandsArgs::join(const std::vector<std::string>& args, Server& server, User* user) {	
	if (args.empty()) {
		return "ERROR: No channel name provided!\r\n";
	}

	std::string channelName = args[0];
	if (!isValidChannelName(channelName, user)) {
		return "ERROR JOIN\r\n";
	}

	createChannelIfNotExists(channelName, server, user);

	Channel* channel = server.getChannels()[channelName];

	createChannelIfNotExists(channelName, server, user);

	/* TODO: ---- APENAS TESTE SEM MODO MODE EXLCUIR APÓS TESTE ------- */
	// Aqui setamos como invite-only manualmente APENAS no primeiro JOIN para usuário dar join sem convite em channel invite-only
	// if (channel->getUsers().size() == 1) {
	// 	channel->setInviteOnly(true);
	// }

	// verifica a permissão do canal como olny invite e se o user tem um convite
	if (!canUserJoinChannel(channel, user, channelName)) {
		return "ERROR JOIN\r\n";
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
}

static void	sendListOfUsers(Channel *channel, User* user) {
	std::vector<User*>& users = channel->getUsers();
	std::string names;
	for (std::vector<User*>::iterator it = users.begin(); it != users.end(); ++it) {
		names += (*it)->getNickName() + " ";
	}
	if (!names.empty())
		names.erase(names.length() - 1);

	std::string nameReply = RPL_NAMREPLY(user->getNickName(), channel->getName(), names);
	sendResponse(user, nameReply);

	std::string endOfNames = RPL_ENDOFNAMES(user->getNickName(), channel->getName());
	sendResponse(user, endOfNames);
}

static bool canUserJoinChannel(Channel* channel, User* user, const std::string& channelName) {
	if (channel->isInviteOnly() && !user->isInvitedTo(channelName)) {
		std::string error = ":ircserver 473 " + user->getNickName() + " " + channelName + " :Cannot join channel (+i)\r\n";
		send(user->getFd(), error.c_str(), error.length(), 0);
		return false;
	}
	return true;
}
