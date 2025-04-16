#include "../../includes/irc.hpp"

std::string CommandsArgs::user(const std::vector<std::string>& args, Server& server, User* user) {
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

    return "User command executed!\r\n";
}