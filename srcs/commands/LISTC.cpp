#include "../../includes/irc.hpp"

std::string CommandsArgs::listc(const std::vector<std::string>& args, Server& server, User* user) {
	(void)args;

	std::ostringstream response;

	response << FTIRC << " 321 " << user->getNickName() << " Channel    |   Users   |   Topics" << CRLF;

	const std::map<std::string, Channel*>& channels = server.getChannels();

    if (channels.empty())
	{
		sendError(user, FTIRC + " 323 " + user->getNickName() + " :No channels available" + CRLF);
		return "";
	}

	for (std::map<std::string, Channel*>::const_iterator it = channels.begin(); it != channels.end(); ++it)
	{
		std::string channelName = it->first;
		Channel* channel = it->second;
		size_t numUsers = channel->getUsers().size();

		response << FTIRC << " 322 " << user->getNickName() << SPACE
				 << channelName << SPACE << numUsers << " :" << channel->getTopic() << CRLF;
	}
    
	response << FTIRC << " 323 " << user->getNickName() << " :End of /LISTC" << CRLF;

	send(user->getFd(), response.str().c_str(), response.str().length(), 0);
	return "";
}
