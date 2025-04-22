#include "../../includes/irc.hpp"

/*
    KICK <channel> <user> [:reason]
*/

static void buildTrailingMessage(std::string& message, const std::vector<std::string>& args, size_t start);
//static bool extractKickParams(std::string& channelName, std::string& targetNick, std::string& reason, const std::vector<std::string>& args);

std::string CommandsArgs::kick(const std::vector<std::string>& args, Server& server, User* user) {

	if (args.size() < 4) {
		sendError(user, ERR_NEEDMOREPARAMS(user->getNickName(), "KICK"));
		return "";
	}

	std::string channelName = args[1];
	if (channelName[0] != '#') {
		sendError(user, ERR_NEEDMOREPARAMS(user->getNickName(), "KICK")); // ou erro custom
		return "";
	}

	std::cout << "channelName: " << channelName << std::endl;

	std::string targetNick = args[2];
	if (targetNick[0] == ':')
		targetNick = targetNick.substr(1);  // Corrige casos em que nick vem com ':'

	std::cout << "targetNick: " << targetNick << std::endl;

	std::string reason;
	if (args.size() >= 4) {
		if (args[3][0] == ':') {
			buildTrailingMessage(reason, args, 3);
		} else {
			reason = args[3];
			for (size_t i = 4; i < args.size(); ++i)
				reason += ' ' + args[i];
		}
	}
	std::cout << "reason: " << reason << std::endl;

    Channel* channel = findChannelInServer(server, user, channelName);
    if (!channel)
	    return "";

    if (!isUserInChannel(*user, *channel)) {
        sendError(user, ERR_NOTONCHANNEL(channelName));
        return "";
    }

    // TODO : verificar se o usuário que mandou KICK é um operador do canal

    User* target = findUserInServer(server, user, targetNick);
	if (!target)
		return "";

    if (!isUserInChannel(*user, *channel)) {
        sendError(user, ERR_USERNOTINCHANNEL(user->getNickName(), targetNick, channelName));
		return "";
	}

    std::string kickMsg;

    if (reason.empty()) {
        kickMsg = RPL_KICKNOREASON(user->getNickName(), user->getUserName(), channelName, target->getNickName());
    } else {
        kickMsg = RPL_KICKREASON(user->getNickName(), user->getUserName(), channelName, target->getNickName(), reason);
    }

    channel->broadcast(kickMsg, target);
    channel->removeUser(target->getFd());

    return "";
}

static void buildTrailingMessage(std::string& message, const std::vector<std::string>& args, size_t start) {
	std::ostringstream oss;
	oss << args[start].substr(1);  // Remove os dois pontos ':'
	for (size_t i = start + 1; i < args.size(); ++i)
		oss << ' ' << args[i];
	message = oss.str();
}

// static bool extractKickParams(std::string& channelName, std::string& targetNick, std::string& reason, const std::vector<std::string>& args) {
// 	if (args.size() < 3)
// 		return false;

// 	channelName = args[0];
// 	if (channelName[0] != '#')
// 		return false;

// 	targetNick = args[1];
// 	if (targetNick[0] == ':')
// 		targetNick = targetNick.substr(1);

// 	if (args[2][0] == ':') {
// 		buildTrailingMessage(reason, args, 2);
// 	} else {
// 		reason = args[2];
// 		for (size_t i = 3; i < args.size(); ++i)
// 			reason += ' ' + args[i];
// 	}

// 	return true;
// }
