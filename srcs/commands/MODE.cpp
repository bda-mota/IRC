#include "../../includes/irc.hpp"

std::string CommandsArgs::mode(const std::vector<std::string>& args, Server& server, User* user) {

    (void)server;
    for (size_t i = 0; i < args.size(); i++) {
        std::cout << "Arg " << i << ": " << args[i] << std::endl;
    }

    // #################################### //

    if (args.size() < 2) {
		sendError(user, ERR_NEEDMOREPARAMS(user->getNickName(), "MODE"));
		return "";
	}

	std::string channelName = args[0];
	std::string modeString = args[1];

    Channel* channel = findChannelInServer(server, user, channelName);
    if (!channel)
        return "";

    if (!channel->isUserInChannel(user)) {
        sendError(user, ERR_NOTONCHANNEL(channelName));
        return "";
    }

	// Verifica se o usuário é operador, pois só operadores podem alterar modos
	if (!channel->isOperator(user)) {
		sendError(user, ERR_CHANOPRIVSNEEDED(user->getNickName(), channelName));
		return "";
	}

	// A partir daqui, você pode começar a interpretar o modeString
	// Por enquanto, só imprime os argumentos pra teste
	std::cout << "| MODE | Canal: " << channelName << " | Modo: " << modeString << std::endl;

	// Em breve: interpretar modeString, aplicar modos e argumentos

    return "";
}