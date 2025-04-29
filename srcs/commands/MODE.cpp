#include "../../includes/irc.hpp"

static std::string showChannelModes(Server& server, User* user, std::string channelName);
static std::string validateExtraArgs(char sign, char mode, const std::vector<std::string>& args);
static void executeMode(Channel* channel, char modeChar, char modeSign, const std::string& extraArg, User* user);

std::string CommandsArgs::mode(const std::vector<std::string>& args, Server& server, User* user) {
    if (args.size() == 1) {
        std::string error = showChannelModes(server, user, args[0]);
        if (!error.empty())
            sendErrorAndLog(user, error);
        return "";
    }

    if (args.size() < 2) {
        sendErrorAndLog(user, ERR_NEEDMOREPARAMS(user->getNickName(), "MODE"));
        return "";
    }

    std::string channelName = args[0];
    Channel* channel = findChannelInServer(server, user, channelName);
    if (!channel) {
        sendErrorAndLog(user, ERR_NOSUCHCHANNEL(channelName));
        return "";
    }

    if (!channel->isUserInChannel(user)) {
        sendErrorAndLog(user, ERR_NOTONCHANNEL(channelName));
        return "";
    }

    if (!channel->isOperator(user)) {
        sendErrorAndLog(user, ERR_CHANOPRISNEEDED(user->getNickName(), channelName));
        return "";
    }

    std::string modeCommands = args[1];
    std::vector<std::string> modes;
    size_t pos = 0;
    while (pos < modeCommands.size()) {
        char sign = modeCommands[pos];
        pos++;
        std::string modeSet = "";
        
        while (pos < modeCommands.size() && (modeCommands[pos] == 'i' || modeCommands[pos] == 't' || modeCommands[pos] == 'k' || modeCommands[pos] == 'l' || modeCommands[pos] == 'o')) {
            modeSet += modeCommands[pos];
            pos++;
        }

        if (!modeSet.empty()) {
            modes.push_back(std::string(1, sign) + modeSet);
        }
    }

    for (std::vector<std::string>::iterator it = modes.begin(); it != modes.end(); ++it) {
        std::string modeSet = *it;
        char modeSign = modeSet[0]; // '+' or '-'
        for (size_t i = 1; i < modeSet.size(); ++i) {
            char modeChar = modeSet[i];
            std::string extraArg = validateExtraArgs(modeSign, modeChar, args);
            if (extraArg.rfind(FTIRC, 0) == 0) {
                sendErrorAndLog(user, extraArg);
                return "";
            }
            executeMode(channel, modeChar, modeSign, extraArg, user);
        }
    }

    return "";
}

static std::string showChannelModes( Server& server, User* user, std::string channelName) {
	Channel* channel = findChannelInServer(server, user, channelName);
	if (!channel) {
		sendErrorAndLog(user, ERR_NOSUCHCHANNEL(channelName));
		return "";
	}

	if (!channel->isUserInChannel(user)) {
		sendErrorAndLog(user, ERR_NOTONCHANNEL(channelName));
		return "";
	}

	std::string modes = channel->getName() + " " + channel->getTopic() + " " + channel->getChannelKey();
	std::string modeParams = "";
	if (channel->isInviteOnly()) {
		modeParams += "i ";
	}
	if (channel->isTopicRestricted()) {
		modeParams += "t ";
	}
	if (channel->getUserLimit() > 0) {
		modeParams += "l ";
	}
	if (channel->hasKey()) {
		modeParams += "k ";
	}
	if (channel->isOperator(user)) {
		modeParams += "o ";
	}
	if (modeParams.empty()) {
		modeParams = "No modes set";
	}
	std::string response = RPL_CHANNELMODEIS(user->getNickName(), channel->getName(), modeParams);
	sendResponse(user, response);
	logger(INFO, user->getNickName() + " requested channel modes for " + channel->getName());
	return "";
}

static std::string validateExtraArgs(char sign, char mode, const std::vector<std::string>& args) {
    if (args.size() <= 2) {
        if ((mode == 'k' || mode == 'l' || mode == 'o') && sign == '+') {
            return ERR_NEEDMOREPARAMS("MODE", "Missing argument for mode '+" + std::string(1, mode) + "'");
        }
        if (mode == 'o' && sign == '-') {
            return ERR_NEEDMOREPARAMS("MODE", "Missing argument for mode '-" + std::string(1, mode) + "'");
        }
        return "";
    }

    if ((mode == 'k' || mode == 'l' || mode == 'o') && sign == '+') {
        if (args.size() > 2) {
            return args[2];
        } else {
            return ERR_NEEDMOREPARAMS("MODE", "Missing argument for mode '+" + std::string(1, mode) + "'");
        }
    }

    if (mode == 'o' && sign == '-') {
        return args[2];
    }
    return "";
}

static void executeMode(Channel* channel, char modeChar, char modeSign, const std::string& extraArg, User* user) {
	std::string error = "";
	switch (modeChar) {
		case 'i':
			inviteOnlyConfig(channel, modeSign);
			channel->broadcastToAll(":" + user->getNickName() + " MODE " + channel->getName() + " " + modeSign + "i\r\n");
			break;

		case 't':
			topicCmdConfig(channel, modeSign);
			channel->broadcastToAll(":" + user->getNickName() + " MODE " + channel->getName() + " " + modeSign + "t\r\n");
			break;

		case 'l':
			error = userLimitConfig(channel, modeSign, extraArg);
			if (error != "")
				sendErrorAndLog(user, error);
			else
				channel->broadcastToAll(":" + user->getNickName()+ " MODE " + channel->getName() + " " + modeSign + "l " + extraArg + "\r\n");
			break;

		case 'k':
			channelKeyConfig(channel, modeSign, extraArg);
			channel->broadcastToAll(":" + user->getNickName() + " MODE " + channel->getName() + " " + modeSign + "k ***" + "\r\n");
			break;

		case 'o':
			channelOpConfig(channel, modeSign, extraArg);
			channel->broadcastToAll(":" + user->getNickName() + " MODE " + channel->getName() + " " + modeSign + "o " + extraArg + "\r\n");
			break;

		default:
			sendErrorAndLog(user, ERR_UNKNOWNMODE(user->getNickName(), std::string(1, modeChar)));
			break;
	}
}