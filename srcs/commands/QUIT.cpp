#include "../../includes/irc.hpp"

static void handleQuitFromChannels(User* user, Server& server, const std::string& quitMessage);

std::string CommandsArgs::quit(const std::vector<std::string>& args, Server& server, User* user) {
	std::string reason;

	if (!args.empty()) {
		reason = args[0];
		if (!reason.empty() && reason[0] == ':')
			reason = reason.substr(1);
		for (size_t i = 1; i < args.size(); i++)
			reason += " " + args[i];
	}

	std::string nick = user->getNickName();
	std::string userName = user->getUserName();
	std::string host = user->getHostName();

	std::string quitMsg = RPL_QUIT(nick, userName, host, reason);

	handleQuitFromChannels(user, server, quitMsg);
	
	logger(INFO, nick + " disconnected from the server.");
	close(user->getFd());
	server.clearUsers(user->getFd());

	return "";
}

static void handleQuitFromChannels(User* user, Server& server, const std::string& quitMessage) {
	std::vector<Channel*> joinedChannels = user->getJoinedChannels();

	for (size_t i = 0; i < joinedChannels.size(); ++i) {
		Channel* channel = joinedChannels[i];

		channel->broadcast(quitMessage, user);
		channel->removeUser(user);

		if (channel->getUsers().empty()) {
			server.getChannels().erase(channel->getName());
			delete channel;
		}
	}

	user->clearJoinedChannels();
}
