#include "../../includes/irc.hpp"

std::string CommandsArgs::mode(const std::vector<std::string>& args, Server& server, User* user) {

    if (args.size() < 2) {
		sendError(user, ERR_NEEDMOREPARAMS(user->getNickName(), "MODE"));
		return "";
	}

	std::string channelName = args[0];
	std::string modeString = args[1];
	// os argumentos adicionais começam no args[2] -> alguns modos precisam obrigatoriamente de argumentos adicionais

    Channel* channel = findChannelInServer(server, user, channelName);
    if (!channel)
        return "";

    if (!channel->isUserInChannel(user)) {
        sendError(user, ERR_NOTONCHANNEL(channelName));
        return "";
    }

	// Verifica se o usuário é operador, pois só operadores podem alterar modos
	if (!channel->isOperator(user)) {
        std::cout << "User " << user->getNickName() << " is not an operator in channel " << channelName << std::endl;
		sendError(user, ERR_CHANOPRISNEEDED(user->getNickName(), channelName));
		return "";
	}

	// Só imprime os argumentos pra teste
	std::cout << "| MODE | Canal: " << channelName << " | Modo: " << modeString << std::endl;

	// Variáveis para controlar o sinal (+ ou -) -> ativar os desativar modos
	char sign = '+';

	// Interpretar a string de modos
	for (size_t i = 0; i < modeString.length(); ++i) {
		char modeChar = modeString[i];

		if (modeChar == '+' || modeChar == '-') {
			sign = modeChar;
			continue;
		}

		(void) sign; // Para evitar warnings de variável não utilizada -> retirar quando começar a implementar os modos

		// A partir daqui, você pode começar a implementar a lógica para adicionar ou remover os modos
		switch (modeChar) {
			case 'i': // invite-only
				std::cout << "Modo 'i' (Invite Only) detectado!" << std::endl;
				// adicionar a lógica para adicionar/remover o modo de convite
				break;
				
			case 't': // tópico restrito
				std::cout << "Modo 't' (Tópico restrito) detectado!" << std::endl;
				// adicionar a lógica para adicionar/remover o modo de tópico restrito
				break;

			case 'l': // limite de membros
				std::cout << "Modo 'l' (Limite de membros) detectado!" << std::endl;
				// adicionar a lógica para adicionar/remover o limite de membros
				break;

			case 'k': // chave de acesso
				std::cout << "Modo 'k' (Chave de acesso) detectado!" << std::endl;
				// adicionar a lógica para adicionar/remover a chave de acesso
				break;

			case 'o': // tornar operador
				std::cout << "Modo 'o' (Tornar operador) detectado!" << std::endl;
				// adicionar a lógica para adicionar/remover o operador
				break;

			default:
				std::cout << "Modo desconhecido: " << modeChar << std::endl;
				break;
		}
	}

    return "";
}