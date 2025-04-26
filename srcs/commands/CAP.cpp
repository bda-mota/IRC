#include "../../includes/irc.hpp"

std::string CommandsArgs::cap(const std::vector<std::string>& args, Server& server, User* user) {
    (void)server;
    (void)user;
	(void)args;
    logger(INFO, "CAP command received by user: " + user->getNickName());

    return "";
}