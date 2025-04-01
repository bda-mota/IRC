#include "../../includes/irc.hpp"

std::string CommandsArgs::pass(const std::vector<std::string>& args) {
    std::cout << "PASS command executed!" << std::endl;
    for (size_t i = 0; i < args.size(); i++) {
        std::cout << "Arg " << i << ": " << args[i] << std::endl;
    }
    return "PASS command executed!\r\n";
}