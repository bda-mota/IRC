#include "../../includes/irc.hpp"

std::string CommandsArgs::part(const std::vector<std::string>& args) {
    std::cout << "PART command executed!" << std::endl;
    for (size_t i = 0; i < args.size(); i++) {
        std::cout << "Arg " << i << ": " << args[i] << std::endl;
    }
    return "PART command executed!\r\n";
}