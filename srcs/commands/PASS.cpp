#include "../../includes/irc.hpp"

std::string CommandsArgs::pass(const std::vector<std::string>& args, Server& server, User* user) {
	user->setHasPassCommand(true);

	std::string errorMsg;
	if (user->isAuth()) {
		sendErrorAndLog(user, ERR_ALREADYREGISTERED(user->getNickName()));
		return "";
	}

	if (args.size() < 1) {
		sendErrorAndLog(user, ERR_NEEDMOREPARAMS("PASS", "Not enough parameters"));
		return "";
	}

	if (args[0] != server.getPassword()) {
		sendErrorAndLog(user, ERR_PASSWDMISMATCH());
		return "";
	}

	user->setAuth(true);
	sendResponse(user, RPL_WELCOME(user->getNickName(), user->getUserName()));
	logger(INFO, user->getNickName() + " authenticated successfully with PASS.");

	if (!user->getRegistered()) {
		sendWelcomeMessage(user);
	}

	return "";
}
