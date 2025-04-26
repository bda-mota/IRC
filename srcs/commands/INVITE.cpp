#include "../../includes/irc.hpp"

static void sendInviteMessages(User* from, User* to, const std::string& channelName);

std::string CommandsArgs::invite(const std::vector<std::string>& args, Server& server, User* user) {
  if (args.size() < 2) {
    sendErrorAndLog(user, ERR_NEEDMOREPARAMS("INVITE", "Not enough parameters"));
    return "";
  }

  std::string targetNick = args[0];
  std::string channelName = args[1];

  User* targetUser = findUserInServer(server, user, targetNick);
  if (!targetUser) {
    sendErrorAndLog(user, ERR_NOSUCHNICK(targetNick));
    return "";
  }

  Channel* channel = findChannelInServer(server, user, channelName);
  if (!channel) {
    sendErrorAndLog(user, ERR_NOSUCHCHANNEL(channelName));
    return "";
  }

  if (!channel->isUserInChannel(user)) {
    sendErrorAndLog(user, ERR_NOTONCHANNEL(channelName));
    return "";
  }

  if (channel->isInviteOnly() && !channel->isOperator(user)) {
    sendErrorAndLog(user, ERR_CHANOPRISNEEDED(user->getNickName(), channelName));
    return "";
  }

  if (channel->isUserInChannel(targetUser)) {
    sendErrorAndLog(user, ERR_USERONCHANNEL(targetNick, channelName));
    return "";
  }

  targetUser->addInvitation(channelName);
  sendInviteMessages(user, targetUser, channelName);
  
  return "";
}
  
static void sendInviteMessages(User* from, User* to, const std::string& channelName) {
  sendResponse(to, RPL_INVITEMSG(from->getNickName(), from->getUserName(), to->getNickName(), channelName));
  sendResponse(from, RPL_INVITING(from->getNickName(), to->getNickName(), channelName));
  logger(INFO, from->getNickName() + " invited " + to->getNickName() + " to " + channelName);
}
