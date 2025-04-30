#include "../../includes/irc.hpp"

static bool isInvalidNick(const std::string& newNick, Server& server, User* user);
static void notifyNickChange(const std::string& oldNick, const std::string& newNick, User* user);

std::string CommandsArgs::nick(const std::vector<std::string>& args, Server& server, User* user) {	
	if (args.empty()) {
		sendErrorAndLog(user, ERR_NONICKNAMEGIVEN());
		return "";
	}
	
	std::string newNick = args[0];
	if (isInvalidNick(newNick, server, user)) {
		sendErrorAndLog(user, ERR_ERRONEUSNICKNAME(newNick));
		return "";
	}
	
	std::string oldNick = user->getNickName();
	user->setNickName(newNick);
	
	if (!user->getNickName().empty()) {
		user->setHasNickCommand(true);
		logger(INFO, user->getNickName() + " set their nickname.");
	}

	notifyNickChange(oldNick, newNick, user);
	
	if (!user->getRegistered()) {
		sendWelcomeMessage(user);
	}

	logger(INFO, user->getNickName() + " changed nickname to " + newNick);
	return "";
}

static bool isInvalidNick(const std::string& newNick, Server& server, User* user) {
	if (isNickInUse(newNick, server.getUsers())) {
		sendErrorAndLog(user, ERR_NICKNAMEINUSE(user->getNickName()));
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
