#include "../../includes/irc.hpp"

std::string CommandsArgs::who(const std::vector<std::string>& args, Server& server, User* user) {
    (void)server;
    (void)user;
    std::cout << "WHO command executed!" << std::endl;
    for (size_t i = 0; i < args.size(); i++) {
        std::cout << "Arg " << i << ": " << args[i] << std::endl;
    }
    return "WHO command executed!\r\n";
}

void	who(Server& server, User* user, const std::string& target) {
	std::vector<User*> targetUsers;

	if (target[0] == '#') {
		std::map<std::string, Channel>& channels = server.getChannels();

		if (channels.find(target) == channels.end()) {
			send(user->getFd(), "Error: channel not found.\r\n", 27, 0);
			return ;
		}

		targetUsers = channels[target].getUsers();
	} else {
		std::vector<User>& allUsers = server.getUsers();
		for (std::vector<User>::iterator it = allUsers.begin(); it != allUsers.end(); ++it) {
			if ((it)->getNickName().find(target) != std::string::npos) {
				targetUsers.push_back(&(*it));
			}
		}
	}

	for (std::vector<User*>::iterator it = targetUsers.begin(); it != targetUsers.end(); it++) {
		User *u = *it;
		std::string response = ":ircserver 352 " + user->getNickName() + " " + target + " " + u->getIP() + " " + u->getNickName() + " H :0 " + u->getUserName() + END;
		send(user->getFd(), response.c_str(), response.length(), 0);
	}
	//channels[target].broadcast(response, user);

	std::string endResponse = ":ircserver 315 " + user->getNickName() + " " + target + " :End of WHO list." + END;
	send(user->getFd(), endResponse.c_str(), endResponse.length(), 0);
}