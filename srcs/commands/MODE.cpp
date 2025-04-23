#include "../../includes/irc.hpp"

static void showChannelModes( Server& server, User* user, std::string channelName);
static std::string validateExtraArgs(char sign, char mode, const std::vector<std::string>& args);

std::string CommandsArgs::mode(const std::vector<std::string>& args, Server& server, User* user) {
  if (args.size() == 1) {
    showChannelModes(server, user, args[0]);
    return "";
  }

	if (args.size() < 2) {
		sendError(user, ERR_NEEDMOREPARAMS(user->getNickName(), "MODE"));
		return "";
	}

	std::string channelName = args[0];

  // checar se o parametro é um modo válido. Sinal + ou - seguido de um char
	if (args[1].size() != 2 || (args[1][0] != '+' && args[1][0] != '-')) {
		sendError(user, ERR_UMODEUNKNOWNFLAG(user->getNickName()));
		return "";
	}

  std::string modeString = args[1];
	char modeSign = args[1][0];
	char modeChar = args[1][1];

  // Verifica se o modo é válido. Os modos válidos são: i, t, k, o e l
	std::string validModes = "itkol";
	if (validModes.find(modeChar) == std::string::npos) {
		sendError(user, ERR_UMODEUNKNOWNFLAG(user->getNickName()));
		return "";
	}

	// os argumentos adicionais começam no args[2] -> alguns modos precisam obrigatoriamente de argumentos adicionais
  // validação dos argumentos adicionais
	std::string extraArg = validateExtraArgs(modeSign, modeChar, args);
  if (extraArg.rfind(FTIRC, 0) == 0) {
    sendError(user, extraArg);
    return "";
  }

  Channel* channel = findChannelInServer(server, user, channelName);
  if (!channel) {
      sendError(user, ERR_NOSUCHCHANNEL(channelName));
      return "";
  }

  if (!channel->isUserInChannel(user)) {
      sendError(user, ERR_NOTONCHANNEL(channelName));
      return "";
  }

	if (!channel->isOperator(user)) {
    std::cout << "User " << user->getNickName() << " is not an operator in channel " << channelName << std::endl;
		sendError(user, ERR_CHANOPRISNEEDED(user->getNickName(), channelName));
		return "";
	}

	// Só imprime os argumentos pra teste
	std::cout << "| MODE | Canal: " << channelName << " | Modo: " << modeSign << modeChar << std::endl;

    std::string error = "";
		switch (modeChar) {
			case 'i':
				std::cout << "Modo 'i' (Invite Only) detectado!" << std::endl;

				inviteOnlyConfig(channel, modeSign);
				channel->broadcast(":" + user->getNickName() + " MODE " + channel->getName() + " " + modeSign + "i\r\n", user);
				break;

			case 't':
				std::cout << "Modo 't' (Tópico restrito) detectado!" << std::endl;
				topicCmdConfig(channel, modeSign);
				channel->broadcast(":" + user->getNickName() + " MODE " + channel->getName() + " " + modeSign + "t\r\n", user);
				break;

			case 'l':
				std::cout << "Modo 'l' (Limite de membros) detectado!" << std::endl;
				
				error = userLimitConfig(channel, modeSign, extraArg);
						if (error != "")
				sendError(user, error);
				else
				channel->broadcast(":" + user->getNickName()+ " MODE " + channel->getName() + " " + modeSign + "l " + extraArg + "\r\n", user);
				break;

			case 'k':
				std::cout << "Modo 'k' (Chave de acesso) detectado!" << std::endl;
				channelKeyConfig(channel, modeSign, extraArg);
				channel->broadcast(":" + user->getNickName() + " MODE " + channel->getName() + " " + modeSign + "k " + extraArg + "\r\n", user);
				break;

			case 'o':
				std::cout << "Modo 'o' (Tornar operador) detectado!" << std::endl;
				channelOpConfig(channel, modeSign, extraArg);
				channel->broadcast(":" + user->getNickName() + " MODE " + channel->getName() + " " + modeSign + "o " + extraArg + "\r\n", user);
				break;

			default:
				std::cout << "Modo desconhecido: " << modeChar << std::endl;
        		sendError(user, ERR_UNKNOWNMODE(user->getNickName(), std::string(1, modeChar)));
				break;
		}

    return "";
}

static void showChannelModes( Server& server, User* user, std::string channelName) {
  Channel* channel = findChannelInServer(server, user, channelName);
  if (!channel) {
      sendError(user, ERR_NOSUCHCHANNEL(channelName));
  }

  if (!channel->isUserInChannel(user)) {
      sendError(user, ERR_NOTONCHANNEL(channelName));
  }

  std::string modes = channel->getName() + " " + channel->getTopic() + " " + channel->getChannelKey();
  std::string modeParams = "";
  if (channel->isInviteOnly()) {
      modeParams += "i ";
  }
  if (channel->isTopicRestricted()) {
      modeParams += "t ";
  }
  if (channel->getUserLimit() > 0) {
      modeParams += "l ";
  }
  if (channel->hasKey()) {
      modeParams += "k ";
  }
  if (channel->isOperator(user)) {
      modeParams += "o ";
  }
  if (modeParams.empty()) {
      modeParams = "No modes set";
  }
  std::string response = RPL_CHANNELMODEIS(user->getNickName(), channel->getName(), modeParams);
  send(user->getFd(), response.c_str(), response.length(), 0);
}

static std::string validateExtraArgs(char sign, char mode, const std::vector<std::string>& args) {
	if (args.size() <= 2) {
		if ((mode == 'k' || mode == 'l' || mode == 'o') && sign == '+') {
			return ERR_NEEDMOREPARAMS("MODE", "Missing argument for mode '+" + std::string(1, mode) + "'");
		}
		if (mode == 'o' && sign == '-') {
			return ERR_NEEDMOREPARAMS("MODE", "Missing argument for mode '-" + std::string(1, mode) + "'");
		}
		return "";
	}

	if ((mode == 'k' || mode == 'l' || mode == 'o') && sign == '+') {
		return args[2];
	}

	if (mode == 'o' && sign == '-') {
		return args[2];
  }
	return "";
}
