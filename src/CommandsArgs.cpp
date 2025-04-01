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
    // _messageFunctions["NICK"] = nick;
    // _messageFunctions["PASS"] = pass;
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

std::string user(const CommandsArgs &args) {
    (void)args;  // Evita warnings sobre argumento não usado
    std::cout << "USER command executed successfully!" << std::endl;
    return "USER command executed successfully!\r\n";
}
