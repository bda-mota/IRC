#include "../../includes/irc.hpp"

std::string CommandsArgs::cap(const std::vector<std::string>& args) {
    std::cout << "CAP command executed!" << std::endl;
    for (size_t i = 0; i < args.size(); i++) {
        std::cout << "Arg " << i << ": " << args[i] << std::endl;
    }
    return "CAP command executed!\r\n";
}