#include "../../includes/irc.hpp"

std::string CommandsArgs::listc(const std::vector<std::string>& args, Server& server, User* user) {
	(void)args;

	std::ostringstream response;

	response << RPL_LISTSTART(user->getNickName());

	const std::map<std::string, Channel*>& channels = server.getChannels();

  if (channels.empty()) {
		sendError(user, RPL_LISTEND(user->getNickName()));
		return "";
	}

	for (std::map<std::string, Channel*>::const_iterator it = channels.begin(); it != channels.end(); ++it)
	{
		std::string channelName = it->first;
		Channel* channel = it->second;
		size_t numUsers = channel->getUsers().size();

		response << RPL_LIST(user->getNickName(), channelName, std::to_string(numUsers), channel->getTopic());
	}

	response << RPL_LISTEND(user->getNickName());

	send(user->getFd(), response.str().c_str(), response.str().length(), 0);
	return "";
}
