#include "../../includes/irc.hpp"

static bool isInvalidNick(const std::string& newNick, Server& server, User* user);
static void notifyNickChange(const std::string& oldNick, const std::string& newNick, User* user);

std::string CommandsArgs::nick(const std::vector<std::string>& args, Server& server, User* user) {
	user->setHasNickCommand(true);

	if (args.empty()) {
		std::string error = ERR_NONICKNAMEGIVEN();
		sendError(user, error);
		return "ERROR NICK" + CRLF;
	}

	std::string newNick = args[0];
	if (isInvalidNick(newNick, server, user)) {
		return "ERROR NICK" + CRLF;
	}

	std::string oldNick = user->getNickName();
	user->setNickName(newNick);

	notifyNickChange(oldNick, newNick, user);

	if (!user->getRegistered()) {
		sendWelcomeMessage(user);
	}

	return "";
}

static bool isInvalidNick(const std::string& newNick, Server& server, User* user) {
	if (isNickInUse(newNick, server.getUsers())) {
		std::string error = ERR_NICKNAMEINUSE(user->getNickName());
		sendError(user, error);
		return true;
	}
	return false;
}

static void notifyNickChange(const std::string& oldNick, const std::string& newNick, User* user) {
	std::string notify = ":" + (oldNick.empty() ? newNick : oldNick) + " NICK :" + newNick + CRLF;

	std::vector<Channel*>& channels = user->getJoinedChannels();
	for (std::vector<Channel*>::iterator it = channels.begin(); it != channels.end(); ++it) {
		if (*it)
			(*it)->broadcast(notify, user);
	}
	sendResponse(user, notify);
}
