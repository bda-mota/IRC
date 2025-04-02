#include "../../includes/irc.hpp"

std::string CommandsArgs::cap(const std::vector<std::string>& args, Server& server, User* user) {
    (void)server;
    (void)user;
    std::cout << "CAP command executed!" << std::endl;
    for (size_t i = 0; i < args.size(); i++) {
        std::cout << "Arg " << i << ": " << args[i] << std::endl;
    }
    return "CAP command executed!\r\n";
}