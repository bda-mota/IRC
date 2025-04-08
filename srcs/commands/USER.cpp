#include "../../includes/irc.hpp"

static void auxBuildRealname(std::string& realname, const std::vector<std::string>& args, size_t start);

std::string CommandsArgs::user(const std::vector<std::string>& args, Server& server, User* user) {
	(void)server; // TODO: manter server aqui? está sem uso

	if (args.size() < 4) {
        std::string error = ":ircserver 461 " + user->getNickName() + " USER :Not enough parameters\r\n";
        send(user->getFd(), error.c_str(), error.length(), 0);
		return "";
	}

    if (user->getRegistered() == true)
    {
        std::string error = ":ircserver 462 " + user->getNickName() + " :You may not reregister\r\n";
		send(user->getFd(), error.c_str(), error.length(), 0);
		return "";
    }

	std::string username = args[0];
	std::string realname;

	if (args.size() >= 4 && args[3][0] == ':')
		auxBuildRealname(realname, args, 3);
	else
    {
        std::string error = ":ircserver 461 " + user->getNickName() + " USER :Missing realname\r\n";
        send(user->getFd(), error.c_str(), error.length(), 0);
        return "";
    }

    user->setUserName(username);
    user->setRealName(realname);
    user->setHasUserCommand(true);

    if (user->getHasUserCommand() == true && user->getHasNickCommand() == true)
    {
        std::string welcomeMessage = ":ircserver 001 " + user->getNickName() + " :Welcome to the IRC server!\r\n";
        send(user->getFd(), welcomeMessage.c_str(), welcomeMessage.length(), 0);
        user->setRegistered(true);
    }

	return "USER command executed successfully.\r\n";
}

static void auxBuildRealname(std::string& realname, const std::vector<std::string>& args, size_t start) {
	for (size_t i = start; i < args.size(); i++) {
		if (i == start)
			realname = args[i].substr(1);
		else
			realname += " " + args[i];
	}
}
