#include "../../includes/irc.hpp"

std::string CommandsArgs::mode(const std::vector<std::string>& args) {
    std::cout << "MODE command executed!" << std::endl;
    for (size_t i = 0; i < args.size(); i++) {
        std::cout << "Arg " << i << ": " << args[i] << std::endl;
    }
    return "MODE command executed!\r\n";
}