#include "../../includes/irc.hpp"

static void	createChannelIfNotExists(const std::string& channelName, Server& server, User* user);
static void	addUserToChannel(Channel* channel, User* user);
static void	sendListOfUsers(Channel *channel, User* user);
static bool canUserJoinChannel(Channel* channel, User* user, const std::string& channelName);

std::string CommandsArgs::join(const std::vector<std::string>& args, Server& server, User* user) {

	if (args.empty()) {
		return "ERROR: No channel name provided!\r\n";
	}

	for (size_t i = 0; i < args.size(); ++i) {
		std::cout << args[i] << std::endl;
		std::string channelName = args[i];
	
		if (!isValidChannelName(channelName, user)) {
			return "ERROR JOIN\r\n";
		}

		createChannelIfNotExists(channelName, server, user);
		Channel* channel = server.getChannels()[channelName];

		// Verifica se o usuário forneceu a senha correta
		if (channel->hasKey()) {
		// Verifica se o usuário forneceu a senha correta
		if (args.size() < 2 || args[1] != channel->getChannelKey()) {
			// Se a senha não for fornecida ou for incorreta, envia erro
			return ERR_BADCHANNELKEY(user->getNickName(), channel->getName());
			}
		}

		// verifica a permissão do canal como olny invite e se o user tem um convite
		if (!canUserJoinChannel(channel, user, channelName)) {
			return "ERROR JOIN\r\n";
		}

		if (channel->isFull()) {
			return ERR_CHANNELISFULL(channelName);
		}

		addUserToChannel(channel, user);
		user->removeInvitation(channelName);
		sendListOfUsers(channel, user);

		std::string notify = JOIN(user->getNickName(), channelName);
		channel->broadcast(notify, user);

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
