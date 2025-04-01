#include "../../includes/irc.hpp"

std::string CommandsArgs::invite(const std::vector<std::string>& args) {
    std::cout << "INVITE command executed!" << std::endl;
    for (size_t i = 0; i < args.size(); i++) {
        std::cout << "Arg " << i << ": " << args[i] << std::endl;
    }
    return "INVITE command executed!\r\n";
}