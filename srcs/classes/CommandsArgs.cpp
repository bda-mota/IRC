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
    _messageFunctions["USER"] = user;
    _messageFunctions["NICK"] = nick;
    _messageFunctions["PASS"] = pass;
    _messageFunctions["CAP"] = cap;
    _messageFunctions["QUIT"] = quit;
    _messageFunctions["JOIN"] = join;
    _messageFunctions["LISTC"] = listc;
    _messageFunctions["PRIVMSG"] = privmsg;
    _messageFunctions["KICK"] = kick;
    _messageFunctions["PART"] = part;
    _messageFunctions["WHO"] = who;
    _messageFunctions["MODE"] = mode;
    _messageFunctions["TOPIC"] = topic;
    _messageFunctions["INVITE"] = invite;
}

std::string CommandsArgs::executeCommand(const std::string& command, const std::vector<std::string>& args, Server& server, User* user) {
    std::map<std::string, funcPtr>::iterator it = _messageFunctions.find(command);
	std::cout << "Command: " << command << std::endl;
    if (it != _messageFunctions.end()) {
        // Passa args, server e user para a função
        return (it->second)(args, server, user);
    }
    return "Comando inválido!\r\n";
}

