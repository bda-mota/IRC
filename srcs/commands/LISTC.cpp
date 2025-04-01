#include "../../includes/irc.hpp"

std::string CommandsArgs::listc(const std::vector<std::string>& args) {
    std::cout << "LISTC command executed!" << std::endl;
    for (size_t i = 0; i < args.size(); i++) {
        std::cout << "Arg " << i << ": " << args[i] << std::endl;
    }
    return "LISTC command executed!\r\n";
}