#include "../includes/CommandParse.hpp"

/* CONSTRUCTORS and DESTRUCTOR */

CommandParse::CommandParse() {}

CommandParse::CommandParse(const CommandParse &other) {
    *this = other;
}

CommandParse &CommandParse::operator=(const CommandParse &other) {
    if (this != &other) {
        // adicionar atributos aqui
    }
    return *this;
}

CommandParse::~CommandParse() {}

/* METHODS */

std::string CommandParse::processCommand(std::string &input) {

    std::istringstream iss(input);
    std::string command;
    std::vector<std::string> args;
    std::string param;  // Variável para armazenar cada argumento temporariamente

    iss >> command;
    while (iss >> param) {
        args.push_back(param);
    }

    std::cout << "input: " << input << std::endl;

    std::cout << "command: " << command << std::endl;

    // Exibe os argumentos
    for (size_t i = 0; i < args.size(); i++) {
        std::cout << "Arg " << i + 1 << ": " << args[i] << std::endl;
    }

    return "Command received in process Command.";
}
