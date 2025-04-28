#include "../../includes/irc.hpp"

std::string CommandsArgs::kick(const std::vector<std::string>& args, Server& server, User* user) {

	if (args.size() < 4 || args[3][0] != ':') {
		sendErrorAndLog(user, ERR_NEEDMOREPARAMS(user->getNickName(), "KICK"));
		return "";
	}

	std::string channelName = args[1];
	if (channelName[0] != '#') {
		sendErrorAndLog(user, ERR_NEEDMOREPARAMS(user->getNickName(), "KICK"));
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
        sendErrorAndLog(user, ERR_NOTONCHANNEL(channelName));
        return "";
    }

	if (!channel->isOperator(user)) {
		sendError(user, ERR_CHANOPRISNEEDED(user->getNickName(), channelName));
		return "";
	}

    User* target = findUserInServer(server, user, targetNick);
	if (!target) {
		sendErrorAndLog(user, ERR_NOSUCHNICK(targetNick));
		return "";
	}

    if (!isUserInChannel(*target, *channel)) {
        sendErrorAndLog(user, ERR_USERNOTINCHANNEL(user->getNickName(), targetNick, channelName));
		return "";
	}

    std::string kickMsg;

    if (reason.empty()) {
        kickMsg = RPL_KICKNOREASON(user->getNickName(), user->getUserName(), channelName, target->getNickName());
    } else {
        kickMsg = RPL_KICKREASON(user->getNickName(), user->getUserName(), channelName, target->getNickName(), reason);
    }

    channel->broadcast(kickMsg, target);
    channel->removeUser(target);
	target->removeChannel(channel);
	logger(INFO, user->getNickName() + " kicked " + target->getNickName() + " from " + channelName + " for reason: " + reason);
    
	return "";
}
