#include "../../includes/irc.hpp"

/*
    KICK <channel> <user> [:reason]
*/

static bool isOperator(Channel* channel, User* user);

std::string CommandsArgs::kick(const std::vector<std::string>& args, Server& server, User* user) {
    (void)server;

    if (args.size() < 2) {
        sendError(user, ERR_NEEDMOREPARAMS(user->getNickName(), "KICK"));
		return "";
	}

    const std::string& channelName = args[0];
	const std::string& targetNick = args[1];
	std::string reason = (args.size() > 2) ? args[2] : "";

    Channel* channel = findChannelInServer(server, user, channelName);
    if (!channel)
	    return "";
    
    if (!isUserInChannel(channel, user)) {
        sendError(user, ERR_NOTONCHANNEL(channelName));
        return "";
    }

    // TODO : verificar se o usuário que mandou KICK é um operador do canal

    User* target = findUserInServer(server, user, targetNick);
	if (!target)
		return "";

    if (!isUserInChannel(channel, target)) {
		sendError(sender, ERR_USERNOTINCHANNEL(targetNick, channelName));
		return "";
	}

    std::string kickMsg;

    if (reason.empty()) {
        kickMsg = RPL_KICKNOREASON(sender->getNickName(), sender->getUserName(), channelName, target->getNickName());
    } else {
        kickMsg = RPL_KICKREASON(sender->getNickName(), sender->getUserName(), channelName, target->getNickName(), reason);
    }

    channel->broadcast(kickMsg, NULL);
    channel->removeUser(target->getFd());

    return "";
}

static bool isOperator(Channel* channel, User* user) {
	const std::vector<User*>& operators = channel->getOperators();
	for (size_t i = 0; i < operators.size(); ++i) {
		if (operators[i] == user)
			return true;
	}
	return false;
}
