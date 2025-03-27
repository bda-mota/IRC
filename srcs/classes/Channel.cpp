#include "../../includes/Channel.hpp"

Channel::Channel() : _name(""), _topic(""), _users(std::vector<User*>()) {}

Channel::Channel(std::string _name) : _name(_name), _users(std::vector<User*>()) {}

Channel::~Channel() {}

Channel::Channel(const Channel &other) { *this = other; }

Channel &Channel::operator=(Channel const &src) {
	if (this != &src) {
		this->_name = src._name;
		this->_topic = src._topic;
		this->_users = src._users;
	}
	return *this;
}

void Channel::setName(std::string name) { _name = name; }

void Channel::setTopic(std::string topic) { _topic = topic; }

std::string Channel::getName() const { return _name; }

std::string Channel::getTopic() const { return _topic; }

std::vector<User*>& Channel::getUsers() { return _users; }

void Channel::addUser(User user) {
	User* newUser = new User(user);
	_users.push_back(newUser);
}

void Channel::removeUser(int fd) {
	for (std::vector<User*>::iterator it = _users.begin(); it != _users.end(); ++it) {
		if ((*it)->getFd() == fd) {
			_users.erase(it);
			break;
		}
	}
}