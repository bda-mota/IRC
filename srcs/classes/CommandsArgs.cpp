#include "../includes/CommandsArgs.hpp"

/* Static member initialization */
std::map<std::string, CommandsArgs::funcPtr> CommandsArgs::_messageFunctions;

/* Constructors and Destructor */
CommandsArgs::CommandsArgs() {}

CommandsArgs::CommandsArgs(const CommandsArgs& other) {
    *this = other;
}

CommandsArgs& CommandsArgs::operator=(const CommandsArgs& other) {
    (void)other;
    return *this;
}

CommandsArgs::~CommandsArgs() {}

/* Método para popular estrutura de comandos */
void CommandsArgs::populateMap() {
    // TODO: implementar a população do map de comandos
    _messageFunctions["USER"] = user;
    _messageFunctions["NICK"] = nick;
    _messageFunctions["PASS"] = pass;
    // _messageFunctions["CAP"] = cap;
    // _messageFunctions["QUIT"] = quit;
    // _messageFunctions["JOIN"] = join;
    // _messageFunctions["LISTC"] = listc;
    // _messageFunctions["PRIVMSG"] = privmsg;
    // _messageFunctions["KICK"] = kick;
    // _messageFunctions["PART"] = part;
    // _messageFunctions["WHO"] = who;
    // _messageFunctions["MODE"] = mode;
    // _messageFunctions["TOPIC"] = topic;
    // _messageFunctions["INVITE"] = invite;
}

std::string CommandsArgs::executeCommand(const std::string &command, const std::vector<std::string> &args) {
    if (_messageFunctions.find(command) != _messageFunctions.end()) {
        return _messageFunctions[command](args);
    }

    return "Command not found!\r\n";
}

/* Métodos para execução de comandos */

std::string CommandsArgs::user(const std::vector<std::string>& args) {
    std::cout << "User command executed!" << std::endl;
    for (size_t i = 0; i < args.size(); i++) {
        std::cout << "Arg " << i << ": " << args[i] << std::endl;
    }

    return "User command executed!\r\n";
}

std::string CommandsArgs::nick(const std::vector<std::string>& args) {
    std::cout << "Nick command executed!" << std::endl;
    for (size_t i = 0; i < args.size(); i++) {
        std::cout << "Arg " << i << ": " << args[i] << std::endl;
    }

    return "Nick command executed!\r\n";
}

std::string CommandsArgs::pass(const std::vector<std::string>& args) {
    std::cout << "Pass command executed!" << std::endl;
    for (size_t i = 0; i < args.size(); i++) {
        std::cout << "Arg " << i << ": " << args[i] << std::endl;
    }

    return "Pass command executed!\r\n";
}