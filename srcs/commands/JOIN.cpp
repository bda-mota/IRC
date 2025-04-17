#include "../../includes/irc.hpp"

static bool	isValidChannelName(const std::string& channelName, User* user);
static void	createChannelIfNotExists(const std::string& channelName, Server& server, User* user);
static void	addUserToChannel(Channel* channel, User* user);
static void	sendListOfUsers(Channel *channel, User* user);

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
	addUserToChannel(channel, user);
	sendListOfUsers(channel, user);

	std::string notify = JOIN(user->getNickName(), channelName);
	channel->broadcast(notify, user);

	return "";
}

static bool	isValidChannelName(const std::string& channelName, User* user) {
	if (channelName.empty() || channelName[0] != '#') {
		std::string error = ERR_NOSUCHCHANNEL(channelName);
		send(user->getFd(), error.c_str(), error.length(), 0);
		return false;
	}
	return true;

}

static void	createChannelIfNotExists(const std::string& channelName, Server& server, User* user) {
	std::map<std::string, Channel*>& channels = server.getChannels();

	if (channels.find(channelName) == channels.end()) {
		channels[channelName] = new Channel(channelName);
		channels[channelName]->addOperator(user);
		std::cout << "canal criado: " << channelName << std::endl;
	}
}

static void	addUserToChannel(Channel* channel, User* user) {
	if (channel->isUserInChannel(user))
    	return;
	channel->addUser(user);
	user->joinChannel(channel);
	std::cout << "User: " << user->getNickName() << " adicionado ao canal: " << channel->getName() << std::endl;

	std::string response = JOIN(user->getNickName(), channel->getName());
	send(user->getFd(), response.c_str(), response.length(), 0);
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
	send(user->getFd(), nameReply.c_str(), nameReply.length(), 0);

	std::string endOfNames = RPL_ENDOFNAMES(user->getNickName(), channel->getName());
	send(user->getFd(), endOfNames.c_str(), endOfNames.length(), 0);
}