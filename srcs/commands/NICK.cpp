#include "../../includes/irc.hpp"

std::string CommandsArgs::nick(const std::vector<std::string>& args, Server& server, User* user) {
	user->setHasNickCommand(true);
	
	if (args.empty()) {
		std::string error = ERR_NONICKNAMEGIVEN();
		send(user->getFd(), error.c_str(), error.length(), 0);
		return "ERROR NICK" + CRLF;
	}
	
	if (isNickInUse(args[0], server.getUsers())) {
		std::string error = ERR_NICKNAMEINUSE(user->getNickName());
		send(user->getFd(), error.c_str(), error.length(), 0);
		return "ERROR NICK" + CRLF;
	}

	std::string oldNick = user->getNickName();
	user->setNickName(args[0]);

	std::string notify = ":" + (oldNick.empty() ? args[0] : oldNick) + " NICK : " + args[0] + CRLF;
	std::vector<Channel*>& channels = user->getJoinedChannels();
	for (std::vector<Channel*>::iterator it = channels.begin(); it != channels.end(); ++it) {
    	if (*it)
			(*it)->broadcast(notify, user);
	}
	send(user->getFd(), notify.c_str(), notify.length(), 0);

	if (!user->getRegistered()) {
		sendWelcomeMessage(user);
	}

	return "NICK command executed!" + CRLF;
}