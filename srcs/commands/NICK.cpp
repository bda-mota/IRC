#include "../../includes/irc.hpp"

std::string CommandsArgs::nick(const std::vector<std::string>& args, Server& server, User* user) {
	if (isNickInUse(args[0], server.getUsers())) {
		std::string error = ":ircserver 433 " + user->getNickName() + " " + args[0] + " :Nickname is already in use" + END;
		send(user->getFd(), error.c_str(), error.length(), 0);
		return "ERROR NICK\r\n";
	}

	std::string oldNick = user->getNickName();
	user->setNickName(args[0]);
	std::string notify = ":" + oldNick + " NICK " + args[0] + END;
	server.broadcast(notify, user);
	return "NICK command executed!\r\n";
}