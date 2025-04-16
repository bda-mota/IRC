#include "../../includes/irc.hpp"

std::string CommandsArgs::nick(const std::vector<std::string>& args, Server& server, User* user) {
	user->setHasNick(true);
	
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
	server.broadcast(notify, user);
	send(user->getFd(), notify.c_str(), notify.length(), 0);

	if (!user->getRegister() && user->getHasUser() && user->getHasNick()) {
		std::string welcome = RPL_WELCOME(user->getNickName(), user->getUserName());
		send(user->getFd(), welcome.c_str(), welcome.length(), 0);
		user->setRegister(true);
	}

	return "NICK command executed!" + CRLF;
}