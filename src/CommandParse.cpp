#include "../includes/CommandParse.hpp"

/* CONSTRUCTORS and DESTRUCTOR */

CommandParse::CommandParse() : _command("") {}

CommandParse::CommandParse(const CommandParse &other) {
    *this = other;
}

CommandParse &CommandParse::operator=(const CommandParse &other) {
    if (this != &other) {
        this->_command = other._command;
    }
    return *this;
}

CommandParse::~CommandParse() {}

/* METHODS */

std::string CommandParse::processCommand(std::string &input) {

    std::istringstream iss(input);
    std::vector<std::string> args;
    std::string param;

    iss >> _command;

    // TODO: extrair os argumentos para o CommandArgs posteriormente
    while (iss >> param) {
        args.push_back(param);
    }
    return "Unknown command.\r\n";
}
