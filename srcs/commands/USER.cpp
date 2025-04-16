#include "../../includes/irc.hpp"

static void auxBuildRealname(std::string& realname, const std::vector<std::string>& args, size_t start);

std::string CommandsArgs::user(const std::vector<std::string>& args, Server& server, User* user) {
	(void)server; // TODO: manter server aqui? está sem uso

    //user->setHasUser(true);

    struct sockaddr_in addr;
    socklen_t addr_len = sizeof(addr);

    if (getpeername(user->getFd(), (struct sockaddr*)&addr, &addr_len) == 0) {
	    std::string hostname = inet_ntoa(addr.sin_addr);
	    user->setHostName(hostname);
    }
    else
	    user->setHostName("unknown");

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

	user->setUserName(args[0]);
    std::string hostname = args[1]; // hostname or IP address não é usado para nada no protocolo, mas capturei caso a gnt precise usar depois
    user->setServerName(args[2]); // server name idem hostname
	std::string realname;

	if (args.size() >= 4 && args[3][0] == ':')
		auxBuildRealname(realname, args, 3);
	else
    {
        std::string error = ":ircserver 461 " + user->getNickName() + " USER :Missing realname\r\n";
        send(user->getFd(), error.c_str(), error.length(), 0);
        return "";
    }
    user->setHasUser(true);
	(void)server;
    (void)user;
    std::cout << "User command executed!" << std::endl;
    for (size_t i = 0; i < args.size(); i++) {
        std::cout << "Arg " << i << ": " << args[i] << std::endl;
    }

	if (!user->getRegister() && user->getHasNick() && user->getHasUser()) {
		std::string welcome = RPL_WELCOME(user->getNickName(), user->getUserName());
		send(user->getFd(), welcome.c_str(), welcome.length(), 0);
		user->setRegister(true);
	}

    user->setRealName(realname);
    user->setHasUserCommand(true);

    if (user->getHasUserCommand() == true && user->getHasNickCommand() == true)
    {
        std::string welcomeMessage = ":ircserver 001 " + user->getNickName() + " :Welcome to the IRC server!\r\n";
        send(user->getFd(), welcomeMessage.c_str(), welcomeMessage.length(), 0);
        user->setRegistered(true);
    }

	return "";
}

static void auxBuildRealname(std::string& realname, const std::vector<std::string>& args, size_t start) {
	for (size_t i = start; i < args.size(); i++) {
		if (i == start)
			realname = args[i].substr(1);
		else
			realname += " " + args[i];
	}
}
