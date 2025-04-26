#include "../../includes/irc.hpp"

static void sendInviteMessages(User* from, User* to, const std::string& channelName);

std::string CommandsArgs::invite(const std::vector<std::string>& args, Server& server, User* user) {

     if (args.size() < 2) {
        sendError(user, ERR_NEEDMOREPARAMS("INVITE", "Not enough parameters"));
        return "";
    }

    std::string targetNick = args[0];
    std::string channelName = args[1];

    User* targetUser = findUserInServer(server, user, targetNick);
    if (!targetUser)
        return "";

    Channel* channel = findChannelInServer(server, user, channelName);
    if (!channel)
        return "";

    // TODO: verificar se o usuário é operador do canal e se é um canal privado após a implementação do MODE

    if (!channel->isUserInChannel(user)) {
        sendError(user, ERR_NOTONCHANNEL(channelName));
        return "";
    }

    if (channel->isUserInChannel(targetUser)) {
        sendError(user, ERR_USERONCHANNEL(targetUser->getNickName(), channelName));
        return "";
    }

    targetUser->addInvitation(channelName);
    sendInviteMessages(user, targetUser, channelName);

    return "";
}

static void sendInviteMessages(User* from, User* to, const std::string& channelName) {
	sendResponse(to, RPL_INVITEMSG(from->getNickName(), from->getUserName(), to->getNickName(), channelName));
	sendResponse(from, RPL_INVITING(from->getNickName(), to->getNickName(), channelName));
}
