#include "../includes/Command.hpp"

/* CONSTRUCTORS and DESTRUCTOR */

Command::Command() : _command("") {}

Command::Command(const Command &other) {
    *this = other;
}

Command &Command::operator=(const Command &other) {
    if (this != &other) {
        this->_command = other._command;
    }
    return *this;
}

Command::~Command() {}

/* METHODS */

std::string Command::processCommand(std::string &input) {

    std::istringstream iss(input);
    std::vector<std::string> args;
    std::string param;

    iss >> _command;
    //TODO: verificar se comando minúsculo é válido
    std::transform(_command.begin(), _command.end(), _command.begin(), ::toupper); // transformando em maiúsculo

    while (iss >> param) {
        args.push_back(param);
    }

    return CommandsArgs::executeCommand(_command, args);
}
