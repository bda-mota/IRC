#include "../../includes/irc.hpp"

std::string CommandsArgs::listc(const std::vector<std::string>& args, Server& server, User* user) {
	(void)args;

	std::ostringstream response;

	response << ":ircserver 321 " << user->getNickName() << " Channel    |   Users   |   Topics\r\n";

	const std::map<std::string, Channel*>& channels = server.getChannels();

    if (channels.empty())
	{
		std::string reply = ":ircserver 323 " + user->getNickName() + " :No channels available\r\n";
		send(user->getFd(), reply.c_str(), reply.length(), 0);
		return "";
	}

	for (std::map<std::string, Channel*>::const_iterator it = channels.begin(); it != channels.end(); ++it)
	{
		std::string channelName = it->first;
		Channel* channel = it->second;
		size_t numUsers = channel->getUsers().size();

		response << ":ircserver 322 " << user->getNickName() << " "
				 << channelName << " " << numUsers << " :" << channel->getTopic() << "\r\n";
	}
    
	response << ":ircserver 323 " << user->getNickName() << " :End of /LISTC\r\n";

	send(user->getFd(), response.str().c_str(), response.str().length(), 0);
	
	return "";
}
