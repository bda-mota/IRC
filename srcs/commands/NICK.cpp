#include "../../includes/irc.hpp"

std::string CommandsArgs::nick(const std::vector<std::string>& args, Server& server, User* user) {
    (void)server;
    (void)user;
    std::cout << "NICK command executed!" << std::endl;
    for (size_t i = 0; i < args.size(); i++) {
        std::cout << "Arg " << i << ": " << args[i] << std::endl;
    }
    return "NICK command executed!\r\n";
}

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