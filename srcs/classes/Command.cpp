#include "../includes/Command.hpp"

/* CONSTRUCTORS and DESTRUCTOR */

Command::Command() {}

Command::Command(const Command &other) {
    *this = other;
}

Command &Command::operator=(const Command &other) {
    (void)other;
    return *this;
}

Command::~Command() {}

/* METHODS */

std::string Command::processCommand(std::string &input, Server& server, User* user) {

    std::istringstream iss(input);
    std::vector<std::string> args;
    std::string param;
    std::string command;

    iss >> command;
    //TODO: verificar se comando minúsculo é válido
    std::transform(command.begin(), command.end(), command.begin(), ::toupper); // transformando em maiúsculo

    while (iss >> param) {
        args.push_back(param);
    }

    return CommandsArgs::executeCommand(command, args, server, user);
}
