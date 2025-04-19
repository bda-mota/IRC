#include "../../includes/irc.hpp"

/*
    KICK <channel> <user> [:reason]
*/

static void buildTrailingMessage(std::string& message, const std::vector<std::string>& args, size_t start);

std::string CommandsArgs::kick(const std::vector<std::string>& args, Server& server, User* user) {

    size_t startIndex = 0;
    while (startIndex < args.size() && args[startIndex][0] != '#') {
        ++startIndex;
    }

    if (args.size() - startIndex < 2) {
        sendError(user, ERR_NEEDMOREPARAMS(user->getNickName(), "KICK"));
        return "";
    }

    std::string channelName = args[startIndex];
    if (channelName[0] != '#') {
        sendError(user, ERR_NEEDMOREPARAMS(user->getNickName(), "KICK"));
        return "";
    }

    std::string targetNick = args[startIndex + 1];
    
    // Verificar se a razão existe, se começar com ":", trataremos como razão
    std::string reason = "";
    if (args.size() > startIndex + 2 && args[startIndex + 2][0] == ':') {
        buildTrailingMessage(reason, args, startIndex + 2);
    }

    std::cout << "Channel Name: " << channelName << std::endl;
    std::cout << "Target Nick: " << targetNick << std::endl;
    std::cout << "Reason: " << reason << std::endl;
   
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
