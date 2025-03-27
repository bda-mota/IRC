#include "../includes/CommandParse.hpp"

/* CONSTRUCTORS and DESTRUCTOR */

CommandParse::CommandParse() : _command("") {
    populateCommandsMap();
}

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

    // Extrai o comando e guarda na variável _command
    iss >> this->_command;

    // TODO: extrair os argumentos para o CommandArgs posteriormente
    while (iss >> param) {
        args.push_back(param);
    }

    if (_commandMap.find(this->_command) != _commandMap.end()) {
        return _commandMap[this->_command](this->_command, args);
    } else {
        return "Unknown command";
    }
}

void CommandParse::populateCommandsMap() {
    _commandMap["USER"] = user;
	_commandMap["NICK"] = nick;
	_commandMap["PASS"] = pass;
	_commandMap["CAP"] = cap;
	_commandMap["QUIT"] = quit;
	_commandMap["JOIN"] = join;
	_commandMap["LISTC"] = listc;
	_commandMap["PRIVMSG"] = privmsg;
	_commandMap["KICK"] = kick;
	_commandMap["PART"] = part;
	_commandMap["WHO"] = who;
	_commandMap["MODE"] = mode;
	_commandMap["TOPIC"] = topic;
	_commandMap["INVITE"] = invite;
}


/* Métodos responsáveis por executar os comandos */
std::string CommandParse::user(const std::string& cmd, std::vector<std::string> args) {
    (void)cmd;
    (void)args;
    return "USER command";
}

std::string CommandParse::nick(const std::string& cmd, std::vector<std::string> args) {
    (void)cmd;
    (void)args;
    return "NICK command";
}

std::string CommandParse::pass(const std::string& cmd, std::vector<std::string> args) {
    (void)cmd;
    (void)args;
    return "PASS command";
}

std::string CommandParse::cap(const std::string& cmd, std::vector<std::string> args) {
    (void)cmd;
    (void)args;
    return "CAP command";
}

std::string CommandParse::quit(const std::string& cmd, std::vector<std::string> args) {
    (void)cmd;
    (void)args;
    return "QUIT command";
}

std::string CommandParse::join(const std::string& cmd, std::vector<std::string> args) {
    (void)cmd;
    (void)args;
    return "JOIN command";
}

std::string CommandParse::listc(const std::string& cmd, std::vector<std::string> args) {
    (void)cmd;
    (void)args;
    return "LISTC command";
}

std::string CommandParse::privmsg(const std::string& cmd, std::vector<std::string> args) {
    (void)cmd;
    (void)args;
    return "PRIVMSG command";
}

std::string CommandParse::kick(const std::string& cmd, std::vector<std::string> args) {
    (void)cmd;
    (void)args;
    return "KICK command";
}

std::string CommandParse::part(const std::string& cmd, std::vector<std::string> args) {
    (void)cmd;
    (void)args;
    return "PART command";
}

std::string CommandParse::who(const std::string& cmd, std::vector<std::string> args) {
    (void)cmd;
    (void)args;
    return "WHO command";
}

std::string CommandParse::mode(const std::string& cmd, std::vector<std::string> args){
    (void)cmd;
    (void)args;
    return "MODE command";
}

std::string CommandParse::topic(const std::string& cmd, std::vector<std::string> args) {
    (void)cmd;
    (void)args;
    return "TOPIC command";
}

std::string CommandParse::invite(const std::string& cmd, std::vector<std::string> args) {
    (void)cmd;
    (void)args;
    return "INVITE command";
}
