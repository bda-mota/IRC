#include "../../includes/irc.hpp"

static bool checkArgs(const std::vector<std::string>& args, User* sender);
static bool	privmsgToChannel(Server& server, User* sender, const std::string& target, const std::string& message);
std::string	buildMessageToSend(const std::vector<std::string>& args);
static bool sendToUser(Server& server, User* sender, const std::string& target, const std::string& message);

std::string CommandsArgs::privmsg(const std::vector<std::string>& args, Server& server, User* sender) {
	if (!checkArgs(args, sender)) {
		return "";
	}
	const std::string& target = args[0];

	std::string message = buildMessageToSend(args);

	if (target[0] == '#') {
		privmsgToChannel(server, sender, target, message);
		return "";
	}

	if (sendToUser(server, sender, target, message)) {
		return "";
	}

	sendErrorAndLog(sender, ERR_NOSUCHNICK(sender->getNickName()));
	return "";
}

static bool checkArgs(const std::vector<std::string>& args, User* sender) {
	if (args.size() < 2) {
		sendErrorAndLog(sender, ERR_NOTEXTTOSEND(sender->getNickName()));
		return false;
	}

	const std::string& target = args[0];

	if (args[1][0] != ':') {
		sendErrorAndLog(sender, ERR_NOTEXTTOSEND(sender->getNickName()));
		return false;
	}

	if (target.empty()) {
		sendErrorAndLog(sender, ERR_NORECIPIENT(sender->getNickName()));
		return false;
	}

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

static bool	privmsgToChannel(Server& server, User* sender, const std::string& target, const std::string& message) {
	std::map<std::string, Channel*>& channels = server.getChannels();
	if (channels.find(target) == channels.end()) {
		sendErrorAndLog(sender, ERR_NOSUCHCHANNEL(target));
		return false;
	}

	Channel* channel = channels[target];
	if (!isUserInChannel(*sender, *channel)) {
		sendErrorAndLog(sender, ERR_CANNOTSENDTOCHAN(sender->getNickName(), target));
		return false;
	}

	if (channel->isInviteOnly() && !channel->isUserInChannel(sender)) {
		sendErrorAndLog(sender, ERR_CANNOTSENDTOCHAN(sender->getNickName(), target));
		return false;
	}

	std::string response = RPL_PRIVMSG(sender->getNickName(), target, message);
	channel->broadcast(response, sender);
	logger(INFO, sender->getNickName() + " sent a message to channel " + target + ": " + message);
	return true;
}

static bool sendToUser(Server& server, User* sender, const std::string& target, const std::string& message) {
	std::vector<User*>& users = server.getUsers();
	for (std::vector<User*>::iterator it = users.begin(); it != users.end(); ++it) {
		User* targetUser = *it;
		if (targetUser->getNickName() == target) {
			std::string response = RPL_PRIVMSG(sender->getNickName(), target, message);
			sendResponse(targetUser, response);
			logger(INFO, sender->getNickName() + " sent a message to user " + target + ": " + message);
			return true;
		}
	}
	return false;
}
