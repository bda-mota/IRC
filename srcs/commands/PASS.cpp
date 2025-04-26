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
<<<<<<< HEAD
		std::cout << "Client " << user->getFd() << " provided wrong password." << std::endl;
		std::string msg = std::string("Wrong password! Could not authenticate client.\n");
		send(user->getFd(), msg.c_str(), msg.length(), 0);
		return ERR_PASSWDMISMATCH();
	}

	user->setAuth(true);
	std::cout << "Client " << user->getFd() << " authenticated successfully with PASS." << std::endl;
	std::string msg = std::string("You've been authenticated!\n");
	send(user->getFd(), msg.c_str(), msg.length(), 0);
	
=======
		sendErrorAndLog(user, ERR_PASSWDMISMATCH());
		return "";
	}

	user->setAuth(true);
	sendResponse(user, RPL_WELCOME(user->getNickName(), user->getUserName()));
	logger(INFO, user->getNickName() + " authenticated successfully with PASS.");

>>>>>>> 77152ee07497a4753f113edc6e6c4a145743b4bb
	if (!user->getRegistered()) {
		sendWelcomeMessage(user);
	}

	return "";
}
