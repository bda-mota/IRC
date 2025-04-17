#include "../../includes/irc.hpp"

std::string CommandsArgs::quit(const std::vector<std::string>& args, Server& server, User* user) {
	std::string reason;

	if (!args.empty()) {
		for (size_t i = 0; i < args.size(); i++) {
			if (i == 0)
				reason = args[i];
			else
				reason += " " + args[i];
		}
	} else {
		reason = "Client Quit";
	}

	std::vector<Channel*> joinedChannels = user->getJoinedChannels();

	std::ostringstream quitMsg;
	
	//:Joao!joao@localhost QUIT :Tô vazando\r\n ----> padrão de mensagem QUIT IRC
	// quitMsg << ":" << user->getNickName() << "!" << user->getUserName()
	//         << "@" << user->getHostName() << " QUIT " << reason << "\r\n";

	std::string host = user->getHostName().empty() ? "localhost" : user->getHostName();

	quitMsg << ":" << user->getNickName() << "!" << user->getUserName()
        << "@" << host << " QUIT " << reason << "\r\n";

	std::cout << "Usuário está em " << joinedChannels.size() << " canais:" << std::endl;
	for (size_t i = 0; i < joinedChannels.size(); ++i) {
		std::cout << "- " << joinedChannels[i]->getName() << std::endl;

	}

	for (size_t i = 0; i < joinedChannels.size(); ++i) {
		Channel* channel = joinedChannels[i];

		channel->sendToAllExcept(quitMsg.str(), user);
		channel->removeUser(user->getFd());

		if (channel->getUsers().empty()) {
			server.getChannels().erase(channel->getName());
			delete channel;
		}
	}

	server.clearUsers(user->getFd());
	close(user->getFd());

	return "bye \r\n";
}
