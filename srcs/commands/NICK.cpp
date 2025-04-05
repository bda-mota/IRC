#include "../../includes/irc.hpp"

std::string CommandsArgs::nick(const std::vector<std::string>& args, Server& server, User* user) {
	if (args.empty()) {
		std::string error = ":ircserver 431 " + user->getNickName() + " :No nickname given" + END;
		send(user->getFd(), error.c_str(), error.length(), 0);
		return "ERROR NICK\r\n";
	}
	
	if (isNickInUse(args[0], server.getUsers())) {
		std::string error = ":ircserver 433 " + user->getNickName() + " " + args[0] + " :Nickname is already in use" + END;
		send(user->getFd(), error.c_str(), error.length(), 0);
		return "ERROR NICK\r\n";
	}

	std::string oldNick = user->getNickName();
	user->setNickName(args[0]);

	std::string notify = ":" + (oldNick.empty() ? args[0] : oldNick) + " NICK :" + args[0] + END;
	server.broadcast(notify, user);
	send(user->getFd(), notify.c_str(), notify.length(), 0); //confirmacao para o usuario
	return "NICK command executed!\r\n";
}