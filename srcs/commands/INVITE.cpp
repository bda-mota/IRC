#include "../../includes/irc.hpp"

/*
    Parameters: <nickname> <channel>
*/

static User* findUserInServer(Server& server, User* sender, const std::string& targetNick);
static Channel* findChannelInServer(Server& server, User* sender, const std::string& channelName);

std::string CommandsArgs::invite(const std::vector<std::string>& args, Server& server, User* user) {

    std::cout << "invite command called" << std::endl;

     if (args.size() < 2) {
        std::string error = ":ircserver 461 " + user->getNickName() + " INVITE :Not enough parameters\r\n";
        send(user->getFd(), error.c_str(), error.length(), 0);
        return "";
    }

    std::string targetNick = args[0];
    std::string channelName = args[1];

    // Procurando usuário no servidor para ser adicionado no channel
    User* targetUser = findUserInServer(server, user, targetNick);
    if (!targetUser)
        return "";

    // Verificando se o canal existe
    Channel* channel = findChannelInServer(server, user, channelName);
    if (!channel)
        return "";

    // TODO: verificar se o usuário é operador do canal e se é um canal privado após a implementação do MODE

    // Verificando se o usuário que faz o convite está no canal
    if (!channel->isUserInChannel(user)) {
        std::string error = ":ircserver 442 " + user->getNickName() + " " + channelName + " :You're not on that channel\r\n";
        send(user->getFd(), error.c_str(), error.length(), 0);
        return "";
    }

    targetUser->addInvitation(channelName);

    // Enviando mensagem de convite para o usuário
    std::string inviteMessage = ":ircserver INVITE " + targetUser->getNickName() + " " + channelName + "\r\n";
    send(targetUser->getFd(), inviteMessage.c_str(), inviteMessage.length(), 0);

    // Enviando mensagem de confirmação para o usuário que fez o convite
    std::string confirmation = ":ircserver 341 " + user->getNickName() + " " + targetNick + " " + channelName + " :Invite sent successfully\r\n";
    send(user->getFd(), confirmation.c_str(), confirmation.length(), 0);

    return "";
}

static User* findUserInServer(Server& server, User* sender, const std::string& targetNick) {
    std::vector<User*>& users = server.getUsers();
    
    for (size_t i = 0; i < users.size(); ++i) {
        if (users[i]->getNickName() == targetNick) {
            return users[i];
        }
    }

    // Usuário não encontrado: envia erro 401
    std::string error = ":ircserver 401 " + sender->getNickName() + " " + targetNick + " :No such nick/channel\r\n";
    send(sender->getFd(), error.c_str(), error.length(), 0);
    return NULL;
}

static Channel* findChannelInServer(Server& server, User* sender, const std::string& channelName) {
    std::map<std::string, Channel*>& channels = server.getChannels();

    if (channels.find(channelName) != channels.end()) {
        return channels[channelName];
    }

    std::string error = ":ircserver 403 " + sender->getNickName() + " " + channelName + " :No such channel\r\n";
    send(sender->getFd(), error.c_str(), error.length(), 0);
    return NULL;
}
