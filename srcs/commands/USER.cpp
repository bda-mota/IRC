#include "../../includes/irc.hpp"

static void auxBuildRealname(std::string& realname, const std::vector<std::string>& args, size_t start);
static std::string getClientHostname(int fd);

std::string CommandsArgs::user(const std::vector<std::string>& args, Server& server, User* user) {
	(void)server; // Unused parameter

	std::string hostname = getClientHostname(user->getFd());
	if (!hostname.empty())
		user->setHostName(hostname);

	if (args.size() < 4) {
		sendError(user, ERR_NEEDMOREPARAMS("USER", "Not enough parameters"));
		return "";
	}

    if (user->getRegistered())
    {
        sendError(user, ERR_ALREADYREGISTERED(user->getNickName()));
		return "";
    }

	user->setUserName(args[0]);
	std::string realname;

	if (args[3][0] != ':') {
        sendError(user, ERR_NEEDMOREPARAMS("USER", "Missing realname"));
        return "";
	}
	auxBuildRealname(realname, args, 3);

	if (!user->getRegistered())
		sendWelcomeMessage(user);

    user->setRealName(realname);

	return "";
}

static void auxBuildRealname(std::string& realname, const std::vector<std::string>& args, size_t start) {
	std::ostringstream oss;
	oss << args[start].substr(1);
	for (size_t i = start + 1; i < args.size(); ++i)
		oss << ' ' << args[i];
	realname = oss.str();
}

static std::string getClientHostname(int fd) {
	struct sockaddr_in addr;
	socklen_t addr_len = sizeof(addr);
	if (getpeername(fd, (struct sockaddr*)&addr, &addr_len) == 0)
		return inet_ntoa(addr.sin_addr);
	return "";
}