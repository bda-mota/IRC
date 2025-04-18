#include "../../includes/irc.hpp"

static std::string createQuitMessage(User* user, const std::string& reason);
static void handleQuitFromChannels(User* user, Server& server, const std::string& quitMessage);

std::string CommandsArgs::quit(const std::vector<std::string>& args, Server& server, User* user) {
	std::string reason;

	if (!args.empty()) {
		for (size_t i = 0; i < args.size(); i++) {
			if (i == 0)
				reason = args[i];
			else
				reason += " " + args[i];
		}
	} else {
		reason = "Client Quit";
	}

	std::vector<Channel*> joinedChannels = user->getJoinedChannels();

	std::string quitMsg = createQuitMessage(user, reason);
	handleQuitFromChannels(user, server, quitMsg);

	server.clearUsers(user->getFd());
	close(user->getFd());

	return "";
}

static std::string createQuitMessage(User* user, const std::string& reason) {
	std::ostringstream quitMsg;
	std::string host = user->getHostName().empty() ? "localhost" : user->getHostName();

	quitMsg << ":" << user->getNickName() << "!" << user->getUserName()
	        << "@" << host << " QUIT :" << reason << CRLF;

	return quitMsg.str();
}

static void handleQuitFromChannels(User* user, Server& server, const std::string& quitMessage) {
	std::vector<Channel*> joinedChannels = user->getJoinedChannels();

	for (size_t i = 0; i < joinedChannels.size(); ++i) {
		Channel* channel = joinedChannels[i];

		channel->broadcast(quitMessage, user);
		channel->removeUser(user->getFd());

		if (channel->getUsers().empty()) {
			server.getChannels().erase(channel->getName());
			delete channel;
		}
	}
}