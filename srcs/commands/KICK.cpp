#include "../../includes/irc.hpp"

std::string CommandsArgs::kick(const std::vector<std::string>& args, Server& server, User* user) {
	
	if (args.size() < 3) {
		sendErrorAndLog(user, ERR_NEEDMOREPARAMS("KICK", "Not enough parameters"));
		return "";
	}

	std::string channelName = args[1];
	if (!isValidChannelName(channelName, user))
		return "";

	std::string targetNick = args[2];
	if (targetNick[0] == ':')
		targetNick = targetNick.substr(1);

	std::string reason = "Kicked for being a bad influence.";

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
	if (!target)
		return "";

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

	// TODO:entender o porquê de não conseguir entrar em um canal novamente após sair

    channel->broadcast(kickMsg, target);
    channel->removeUser(target);
	target->removeChannel(channel);
	logger(INFO, user->getNickName() + " kicked " + target->getNickName() + " from " + channelName + " for reason: " + reason);
    
	return "";
}
