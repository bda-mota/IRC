#include "../../includes/Channel.hpp"

Channel::Channel() : _name(""), _topic(""), _channelUsers(std::vector<User*>()) {}

Channel::Channel(std::string _name) : _name(_name), _channelUsers(std::vector<User*>()) {}

Channel::~Channel() {
	_channelUsers.clear();
}

Channel::Channel(Channel const& other) { *this = other; }

Channel &Channel::operator=(Channel const &other) {
	if (this != &other) {
		this->_name = other._name;
		this->_topic = other._topic;

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

void Channel::setTopic(std::string topic) { _topic = topic; }

const std::string& Channel::getName() const { return _name; }

const std::string& Channel::getTopic() const { return _topic; }

std::vector<User*>& Channel::getUsers() { return _channelUsers; }

bool Channel::isUserInChannel(User* user) const {
	return std::find(_channelUsers.begin(), _channelUsers.end(), user) != _channelUsers.end();
}

void Channel::addUser(User* user) { 
	if (!isUserInChannel(user)) {
		_channelUsers.push_back(user); 
	}
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
	std::vector<User*> users = this->getUsers();

	for (std::vector<User*>::iterator it = users.begin(); it != users.end(); ++it) {
		if ((*it)->getFd() != sender->getFd()) {
			send((*it)->getFd(), message.c_str(), message.length(), 0);
		}
	}
}

bool Channel::isAdmin(User* user) const {
	return _admins.find(user) != _admins.end();
}

void Channel::addAdmin(User* user) {
	if (user) {
		_admins.insert(user); // como o set nao permite duplicatas só isso já é o suficinete
	}
}

void Channel::removeAdmin(User* user) {
	_admins.erase(user);
}

void Channel::sendToAllExcept(const std::string& message, User *excludedUser) {
	for (std::vector<User*>::iterator it = _channelUsers.begin(); it != _channelUsers.end(); ++it) {
		if (*it != excludedUser) {
			send((*it)->getFd(), message.c_str(), message.length(), 0);
		}
	}
}
