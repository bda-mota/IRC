#include "Commands.hpp"

/* Static member initialization */
std::map<std::string, Commands::funcPtr> Commands::_messageFunctions;

/* Constructors and Destructor */
Commands::Commands() {}

Commands::Commands(const Commands& other) {
    *this = other;
}

Commands& Commands::operator=(const Commands& other) {
    (void)other;
    return *this;
}

Commands::~Commands() {}

/* Método para popular estrutura de comandos */
void Commands::populateMap() {
    // TODO: implementar a população do map de comandos
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