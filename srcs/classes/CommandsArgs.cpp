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

void CommandsArgs::populateMap() {
    _messageFunctions["USER"] = user;
    _messageFunctions["NICK"] = nick;
    _messageFunctions["PASS"] = pass;
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
    _messageFunctions["CAP"] = cap;
}

std::string CommandsArgs::executeCommand(const std::string& command, const std::vector<std::string>& args, Server& server, User* user) {
	if ( !user->isAuth() && command != "PASS" && command != "CAP") {
		return "ERROR: You are not authenticated.\r\n";
	}

	if (!user->getRegistered() &&
		command != "NICK" && command != "USER" && command != "PASS" && command != "CAP") {
		return "ERROR: You are not registered.\r\n";
	}

	std::map<std::string, funcPtr>::iterator it = _messageFunctions.find(command);
    if (it != _messageFunctions.end()) {
        return (it->second)(args, server, user);
    }

    sendErrorAndLog(user, ERR_UNKNOWNCOMMAND(user->getNickName(), command));
    return "";
}
