#include "../../includes/irc.hpp"

// TODO: alterar o retorno para void
std::string CommandsArgs::join(const std::vector<std::string>& args, Server& server, User* user) {	
	if (args.empty() ) {
        return "ERROR: No channel name provided!\r\n";
    }

    std::string channelName = args[0];
	if (channelName[0] != '#') {
		std::string error = ":ircserver 403 " + user->getNickName() + " " + channelName + " :No such channel" + END; // mudar essa mensagem
		send(user->getFd(), error.c_str(), error.length(), 0);
		return "ERROR JOIN\r\n";
	}

	std::map<std::string, Channel*>& channels = server.getChannels();

	if (channels.find(channelName) == channels.end()) {
		channels.insert(std::make_pair(channelName, new Channel(channelName)));
		channels[channelName]->addAdmin(user);
	}

	channels[channelName]->addUser(user);
	user->joinChannel(channels[channelName]);
	
	std::string response = ":" + user->getNickName() + "JOIN " + channelName + "\r\n";
	send(user->getFd(), response.c_str(), response.length(), 0);
	
	std::vector<User*>& users = channels[channelName]->getUsers();
	for (std::vector<User*>::iterator it = users.begin(); it != users.end(); ++it) {
		User* currentUser = *it;
		if (currentUser->getFd() != user->getFd()) {
			std::string notify = ":" + user->getNickName() + " entrou no canal " + channelName + END;
			send(currentUser->getFd(), notify.c_str(), notify.length(), 0);
		}
	}
	return "JOIN command executed!\r\n";
	//channels[channelName].broadcast(response, user);
}