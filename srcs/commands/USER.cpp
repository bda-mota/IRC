#include "../../includes/irc.hpp"

std::string CommandsArgs::user(const std::vector<std::string>& args) {
    std::cout << "User command executed!" << std::endl;
    for (size_t i = 0; i < args.size(); i++) {
        std::cout << "Arg " << i << ": " << args[i] << std::endl;
    }

    return "User command executed!\r\n";
}