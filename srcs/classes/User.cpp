#include "../../includes/User.hpp"

User::User() : _fd(-1), _IP(""), _userName(""), _nickName("") , _hasUserCommand(false), _hasNickCommand(false), _hasRegistered(false) {}

User::User(int fd, std::string ip, std::string userName, std::string nickName) : _fd(fd), _IP(ip), _userName(userName),  _nickName(nickName){}

User::~User() {}

int User::getFd() const { return _fd; }

const std::string& User::getIP() const { return _IP; }

const std::string& User::getUserName() const { return _userName; }

const std::string& User::getNickName() const { return _nickName; }

const std::string& User::getRealName() const { return _realName; }

bool User::getHasUserCommand() const { return _hasUserCommand; }
bool User::getHasNickCommand() const { return _hasNickCommand; }
bool User::getRegistered() const { return _hasRegistered; }

const std::vector<Channel*>& User::getJoinedChannels() const { return _joinedChannels; }

std::vector<Channel*>& User::getJoinedChannels() { return _joinedChannels; }

void User::setFd(int fd) { _fd = fd; }

void User::setIP(std::string const& IP) { _IP = IP; }

void User::setUserName(std::string const& userName) { _userName = userName; }

void User::setNickName(std::string const& nickName) { _nickName = nickName; }

void User::setRealName(std::string const& realName) { _realName = realName; }

void User::setHasUserCommand(bool hasUserCommand) { _hasUserCommand = hasUserCommand; }
void User::setHasNickCommand(bool hasNickCommand) { _hasNickCommand = hasNickCommand; }
void User::setRegistered(bool registered) { _hasRegistered = registered; }

void User::joinChannel(Channel* channel) {
	if (!isInChannel(channel)) {
		_joinedChannels.push_back(channel);
	}
}

bool User::isInChannel(Channel* channel) const {
	for (size_t i = 0; i < _joinedChannels.size(); i++) {
		if (_joinedChannels[i] == channel) {
			return true;
		}
	}
	return false;
}