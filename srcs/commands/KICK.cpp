#include "../../includes/irc.hpp"

std::string CommandsArgs::kick(const std::vector<std::string>& args, Server& server, User* user) {
	if (args.size() < 2) {
		sendErrorAndLog(user, ERR_NEEDMOREPARAMS("KICK", "Not enough parameters"));
		return "";
	}

  std::string channelName = "";
  size_t pos = 0;
  for (size_t i = 0; i < args.size(); ++i)
  {
      if (!args[i].empty() && args[i][0] == '#')
      {
          channelName = args[i];
          pos = ++i;
          break;
      }
  }

	if (!isValidChannelName(channelName, user))
		return "";

	std::string targetNick = args[pos];
	if (targetNick[0] == ':')
		targetNick = targetNick.substr(1);

	std::string reason = "Kicked for being a bad influence.";

    Channel* channel = findChannelInServer(server, user, channelName);
    if (!channel)
	    return "";
	
	if (user->getNickName() == targetNick) {
		sendErrorAndLog(user, ERR_CANTKICKYOURSELF(user->getNickName(), channel->getName()));
		return "";
	}

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

	std::string kickMsg =  RPL_KICKREASON(user->getNickName(), user->getUserName(), channelName, target->getNickName(), reason);
	send(target->getFd(), kickMsg.c_str(), kickMsg.length(), 0);

    channel->broadcast(kickMsg, target);
    channel->removeUser(target);
	target->removeChannel(channel);

	if (channel->isOperator(target))
		channel->removeOperator(target);
	
	logger(INFO, user->getNickName() + " kicked " + target->getNickName() + " from " + channelName + " for reason: " + reason);

	return "";
}
