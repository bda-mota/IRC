#include "../../includes/irc.hpp"

std::string CommandsArgs::kick(const std::vector<std::string>& args, Server& server, User* user) {
    (void)server;
    (void)user;
    std::cout << "KICK command executed!" << std::endl;
    for (size_t i = 0; i < args.size(); i++) {
        std::cout << "Arg " << i << ": " << args[i] << std::endl;
    }
    return "";
}