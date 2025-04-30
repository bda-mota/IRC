#include "../../includes/irc.hpp"

static std::string getClientHostname(int fd);

std::string CommandsArgs::user(const std::vector<std::string>& args, Server& server, User* user) {
	(void)server;
	
	std::string hostname = getClientHostname(user->getFd());
	if (!hostname.empty())
	user->setHostName(hostname);
	
	if (args.size() < 4) {
		sendErrorAndLog(user, ERR_NEEDMOREPARAMS("USER", "Not enough parameters"));
		return "";
	}
	
	if (user->getRegistered()) {
		sendErrorAndLog(user, ERR_ALREADYREGISTERED(user->getNickName()));
		return "";
	}
	
	user->setUserName(args[0]);
	if (!user->getUserName().empty()) {
		user->setHasUserCommand(true);
	}
		
	std::string realname;

	if (args[3][0] != ':') {
		sendErrorAndLog(user, ERR_NEEDMOREPARAMS("USER", "Missing realname"));
		return "";
	}
	buildTrailingMessage(realname, args, 3);

	if (!user->getRegistered())
		sendWelcomeMessage(user);

  	user->setRealName(realname);

	logger(INFO, user->getNickName() + " registered with USER command.");

	return "";
}

static std::string getClientHostname(int fd) {
	struct sockaddr_in addr;
	socklen_t addr_len = sizeof(addr);
	if (getpeername(fd, (struct sockaddr*)&addr, &addr_len) == 0)
		return inet_ntoa(addr.sin_addr);
	return "";
}
