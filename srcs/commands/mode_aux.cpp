#include "../../includes/irc.hpp"

void inviteOnlyConfig(Channel* channel, char sign) {
	if (sign == '+')
		channel->setInviteOnly(true);
	else
		channel->setInviteOnly(false);
}

void topicCmdConfig(Channel* channel, char modeSign) {
	if (modeSign == '+') {
		channel->setTopicRestricted(true);
	} else if (modeSign == '-') {
		channel->setTopicRestricted(false);
	}
}

void channelKeyConfig(Channel* channel, char modeSign, const std::string& extraArg) {
	if (modeSign == '+') {
		channel->setChannelKey(extraArg);
	} else if (modeSign == '-') {
		channel->setChannelKey("");
	}
}

void channelOpConfig(Channel* channel, char modeSign, const std::string& extraArg) {
	if (modeSign == '+') {
		User* user = channel->getUserByNick(extraArg);
		if (user) {
			channel->addOperator(user);
		}
	} else if (modeSign == '-') {
		User* user = channel->getUserByNick(extraArg);
		if (user) {
			channel->removeOperator(user);
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
      } else if (modeSign == '-') {
          channel->setUserLimit(0);
      }
  } catch (const std::invalid_argument& e) {
      return ERR_NEEDMOREPARAMS("LIMIT", "Invalid argument for user limit.");
  }
  return "";
}
