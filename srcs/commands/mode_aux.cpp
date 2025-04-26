#include "../../includes/irc.hpp"

void inviteOnlyConfig(Channel* channel, char sign) {
	if (sign == '+') {
		channel->setInviteOnly(true);
		logger(INFO, "Channel " + channel->getName() + " is now invite-only.");
	} else {
		channel->setInviteOnly(false);
		logger(INFO, "Channel " + channel->getName() + " is no longer invite-only.");
	}
}

void topicCmdConfig(Channel* channel, char modeSign) {
	if (modeSign == '+') {
		channel->setTopicRestricted(true);
		logger(INFO, "Topic command is now restricted for channel " + channel->getName());
	} else if (modeSign == '-') {
		channel->setTopicRestricted(false);
		logger(INFO, "Topic command is no longer restricted for channel " + channel->getName());
	}
}

void channelKeyConfig(Channel* channel, char modeSign, const std::string& extraArg) {
	if (modeSign == '+') {
		channel->setChannelKey(extraArg);
		logger(INFO, "Channel key set to " + extraArg + " for channel " + channel->getName());
	} else if (modeSign == '-') {
		channel->setChannelKey("");
		logger(INFO, "Channel key removed for channel " + channel->getName());
	}
}

void channelOpConfig(Channel* channel, char modeSign, const std::string& extraArg) {
	if (modeSign == '+') {
		User* user = channel->getUserByNick(extraArg);
		if (user) {
			channel->addOperator(user);
			logger(INFO, "User " + user->getNickName() + " is now an operator in channel " + channel->getName());
		}
	} else if (modeSign == '-') {
		User* user = channel->getUserByNick(extraArg);
		if (user) {
			channel->removeOperator(user);
			logger(INFO, "User " + user->getNickName() + " is no longer an operator in channel " + channel->getName());
		}
	}
}

std::string userLimitConfig(Channel* channel, char modeSign, const std::string& extraArg) {
  	try {
    	if (modeSign == '+') {
			int limit = std::atoi(extraArg.c_str());

			if (limit < 0) {
				return ERR_CHANNELISFULL("*");
			}

			channel->setUserLimit(limit);
			std::ostringstream oss;
			oss << limit;
			logger(INFO, "User limit set to " + oss.str() + " for channel " + channel->getName());
		} else if (modeSign == '-') {
			channel->setUserLimit(0);
			logger(INFO, "User limit removed for channel " + channel->getName());
		}
	} catch (const std::invalid_argument& e) {
		return ERR_NEEDMOREPARAMS("LIMIT", "Invalid argument for user limit.");
	}
	return "";
}
