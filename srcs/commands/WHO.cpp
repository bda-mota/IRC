#include "../../includes/irc.hpp"

std::string CommandsArgs::who(const std::vector<std::string>& args, Server& server, User* user) {
    (void)server;
    (void)user;
    std::cout << "WHO command executed!" << std::endl;
    for (size_t i = 0; i < args.size(); i++) {
        std::cout << "Arg " << i << ": " << args[i] << std::endl;
    }
    return "WHO command executed!\r\n";
}