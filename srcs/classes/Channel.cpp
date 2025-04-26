#include "../../includes/Channel.hpp"
#include "../../includes/irc.hpp"

Channel::Channel()
    : _name(""),
      _topic(""),
      _inviteOnly(false),
      _topicRestricted(true),
      _userLimit(0),
      _channelKey(""),
      _channelUsers(std::vector<User*>()),
      _operators(std::set<User*>()) {}

Channel::Channel(std::string _name)
    : _name(_name),
      _topic(""),
      _inviteOnly(false),
      _topicRestricted(true),
      _userLimit(0),
      _channelKey(""),
      _channelUsers(std::vector<User*>()),
      _operators(std::set<User*>()) {}

Channel::~Channel() {
	_channelUsers.clear();
}

Channel::Channel(Channel const& other) { *this = other; }

Channel &Channel::operator=(Channel const &other) {
  if (this != &other) {
      this->_name = other._name;
      this->_topic = other._topic;
      this->_inviteOnly = other._inviteOnly;
      this->_topicRestricted = other._topicRestricted;
      this->_userLimit = other._userLimit;
      this->_channelKey = other._channelKey;
      this->_operators = other._operators;

      for (size_t i = 0; i < _channelUsers.size(); i++) {
          delete _channelUsers[i];
      }
      _channelUsers.clear();

      for (size_t i = 0; i < other._channelUsers.size(); i++) {
          this->_channelUsers.push_back(new User(*other._channelUsers[i]));
      }
  }
  return *this;
}

void Channel::setName(std::string name) { _name = name; }

std::string Channel::setTopic(std::string topic, User* user) {
  if (this->isTopicRestricted() && !this->isOperator(user)) {
      return ERR_CHANOPRISNEEDED(user->getNickName(), this->getName());
  }

  _topic = topic;
  return "";
}

const std::string& Channel::getName() const { return _name; }

const std::string& Channel::getTopic() const { return _topic; }

std::vector<User*>& Channel::getUsers() { return _channelUsers; }

bool Channel::isUserInChannel(User* user) const {
	return std::find(_channelUsers.begin(), _channelUsers.end(), user) != _channelUsers.end();
}

std::string Channel::addUser(User* user, const std::string& key) {
  if (_userLimit > 0 && _channelUsers.size() >= static_cast<size_t>(_userLimit)) {
      return ERR_CHANNELISFULL(_name);
  }

  if (hasKey() && key != _channelKey) {
      return ERR_BADCHANNELKEY(user->getNickName(), _name);
  }

  if (!isUserInChannel(user)) {
      _channelUsers.push_back(user);
  }

  return "";
}

void Channel::removeUser(int fd) {
	for (std::vector<User*>::iterator it = _channelUsers.begin(); it != _channelUsers.end(); ) {
		if ((*it)->getFd() == fd) {
			it = _channelUsers.erase(it);
			break;
		} else {
			++it;
		}
	}
}

void Channel::broadcast(const std::string& message, User* sender) {
	if (!sender) return;

	for (std::vector<User*>::iterator it = _channelUsers.begin(); it != _channelUsers.end(); ++it) {
		User* target = *it;
		if (target && target->getFd() > 0 && target != sender) {
			send(target->getFd(), message.c_str(), message.length(), 0);
		}
	}
}

void Channel::broadcastToAll(const std::string& message) {
  for (std::vector<User*>::iterator it = _channelUsers.begin(); it != _channelUsers.end(); ++it) {
      User* target = *it;
      if (target && target->getFd() > 0) {
          send(target->getFd(), message.c_str(), message.length(), 0);
      }
  }
}

bool Channel::isOperator(User* user) const {
	return _operators.find(user) != _operators.end();
}

void Channel::addOperator(User* user) {
  std::cout << "Adding operator: " << user->getNickName() << std::endl;
	if (user) {
		_operators.insert(user); // como o set nao permite duplicatas só isso já é o suficinete
	}
}

void Channel::removeOperator(User* user) {
	_operators.erase(user);
}

// Invite only

bool Channel::isInviteOnly() const { return _inviteOnly; }
void Channel::setInviteOnly(bool inviteOnly) { _inviteOnly = inviteOnly; }


// Topic Restricted
bool Channel::isTopicRestricted() const { return _topicRestricted; }
void Channel::setTopicRestricted(bool topicRestricted) { _topicRestricted = topicRestricted; }

// User Limit
int Channel::getUserLimit() const { return _userLimit; }

std::string Channel::setUserLimit(int newLimit) {
  _userLimit = newLimit;
  return "";
}

bool Channel::isFull() const {
  return _userLimit > 0 && _channelUsers.size() >= static_cast<size_t>(_userLimit);
}

// Channel Key
std::string Channel::getChannelKey() const { return _channelKey; }
void Channel::setChannelKey(const std::string& key) { _channelKey = key; }
bool Channel::hasKey() const { return !_channelKey.empty(); }


// User by Nick
User* Channel::getUserByNick(const std::string& nickname) const {
  for (std::vector<User*>::const_iterator it = _channelUsers.begin(); it != _channelUsers.end(); ++it) {
    if ((*it)->getNickName() == nickname) {
      return *it;
    }
  }

  return NULL;
}
