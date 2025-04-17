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
    std::string param; // argumento do comando 
    std::string command; //primeiro argumento do comando

    iss >> command;
    //TODO: pode ser minusculo ou maiusculo
    std::transform(command.begin(), command.end(), command.begin(), ::toupper); // transformando em maiúsculo

	while (iss >> param) {
		args.push_back(param);
		std::cout << "param: " << param << std::endl;
	}

    return CommandsArgs::executeCommand(command, args, server, user);
}
