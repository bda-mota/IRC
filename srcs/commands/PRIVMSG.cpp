#include "../../includes/irc.hpp"

static bool checkArgs(const std::vector<std::string>& args, User* sender);
static bool	privmsgToChannel(Server& server, User* sender, const std::string& target, const std::string& message);
std::string	buildMessageToSend(const std::vector<std::string>& args);
static bool sendToUser(Server& server, User* sender, const std::string& target, const std::string& message);

std::string CommandsArgs::privmsg(const std::vector<std::string>& args, Server& server, User* sender) {
	if (!checkArgs(args, sender)) {
		return "ERROR: Invalid arguments\r\n";
	}
	const std::string& target = args[0];

	std::string message = buildMessageToSend(args);
	
	if (target[0] == '#') {
		privmsgToChannel(server, sender, target, message);
		return "PRIVMSG on channel command executed!\r\n";
	}

	if (sendToUser(server, sender, target, message)) {
		return "PRIVMSG to user command executed!\r\n";
	}

	std::string error = ERR_NOSUCHNICK(sender->getNickName());
	sendError(sender, error);
	return "ERROR: No such nick\r\n";
}

static bool checkArgs(const std::vector<std::string>& args, User* sender) {
	if (args.size() < 2) {
		std::string error = ERR_NOTEXTTOSEND(sender->getNickName());
		sendError(sender, error);
		return false;
	}

	const std::string& target = args[0];

	if (args[1][0] != ':') {
		std::string error = ERR_NOTEXTTOSEND(sender->getNickName());
		sendError(sender, error);
		return false;
	}

	if (target.empty()) {
		std::string error = ERR_NORECIPIENT(sender->getNickName());
		sendError(sender, error);
		return false;
	}

	return true;
}

static bool	privmsgToChannel(Server& server, User* sender, const std::string& target, const std::string& message) {
	std::map<std::string, Channel*>& channels = server.getChannels();
	if (channels.find(target) == channels.end()) {
		std::string error = ERR_NOSUCHCHANNEL(target);
		sendError(sender, error);
		return false;
	}

	Channel* channel = channels[target];
	if (!isUserInChannel(*sender, *channel)) {
		std::string error =  ERR_CANNOTSENDTOCHAN(sender->getNickName(), target);
		send(sender->getFd(), error.c_str(), error.length(), 0);
		return false;
	}

	std::string response = RPL_PRIVMSG(sender->getNickName(), target, message);
	channel->broadcast(response, sender);
	return true;
}

std::string	buildMessageToSend(const std::vector<std::string>& args) {
	std::string message;
	for (size_t i = 1; i < args.size(); ++i) {
		message += args[i];
		if (i != args.size() - 1)
			message += " ";
	}
	if (!message.empty() && message[0] == ':')
		message = message.substr(1);

	return message;
}

static bool sendToUser(Server& server, User* sender, const std::string& target, const std::string& message) {
	std::vector<User*>& users = server.getUsers();
	for (std::vector<User*>::iterator it = users.begin(); it != users.end(); ++it) {
		User* targetUser = *it;
		if (targetUser->getNickName() == target) {
			std::string response = RPL_PRIVMSG(sender->getNickName(), target, message);
			sendResponse(targetUser, response);
			return true;
		}
	}
	return false;
}