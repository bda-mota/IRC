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

void Channel::addUser(User* user) { _channelUsers.push_back(user); }

void Channel::removeUser(int fd) {
	for (std::vector<User*>::iterator it = _channelUsers.begin(); it != _channelUsers.end(); ) {
		if ((*it)->getFd() == fd) {
			delete *it;
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
