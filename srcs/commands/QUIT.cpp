#include "../../includes/irc.hpp"

std::string CommandsArgs::quit(const std::vector<std::string>& args) {
    std::cout << "QUIT command executed!" << std::endl;
    for (size_t i = 0; i < args.size(); i++) {
        std::cout << "Arg " << i << ": " << args[i] << std::endl;
    }
    return "QUIT command executed!\r\n";
}