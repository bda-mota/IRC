#include "../../includes/irc.hpp"

void	nick(Server& server, User* user, const std::string& newNick) {
	if (isNickInUse(newNick, server.getUsers())) {
		std::string error = ":ircserver 433 " + user->getNickName() + " " + newNick + " :Nickname is already in use" + END;
		send(user->getFd(), error.c_str(), error.length(), 0);
		return;
	}

	std::string oldNick = user->getNickName();
	user->setNickName(newNick);
	std::string notify = ":" + oldNick + " NICK " + newNick + END;
	server.broadcast(notify, user);
}