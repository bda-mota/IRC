#include "../../includes/irc.hpp"

std::string CommandsArgs::kick(const std::vector<std::string>& args, Server& server, User* user) {

	if (args.size() < 4 || args[3][0] != ':') {
		sendError(user, ERR_NEEDMOREPARAMS("KICK", "Not enough parameters"));
		return "";
	}

	std::string channelName = args[1];
	if (channelName[0] != '#') {
		sendError(user, ERR_NEEDMOREPARAMS("KICK", "Invalid channel name"));
		return "";
	}

	std::string targetNick = args[2];
	if (targetNick[0] == ':')
		targetNick = targetNick.substr(1);

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

    Channel* channel = findChannelInServer(server, user, channelName);
    if (!channel)
	    return "";

    if (!isUserInChannel(*user, *channel)) {
        sendError(user, ERR_NOTONCHANNEL(channelName));
        return "";
    }

	if (!channel->isOperator(user)) {
		sendError(user, ERR_CHANOPRISNEEDED(user->getNickName(), channelName));
		return "";
	}

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
