#include "../../includes/Channel.hpp"

Channel::Channel(std::string _name) : _name(_name), _clients(std::vector<Client>()) {}

Channel::~Channel() {}

Channel::Channel(const Channel &other) { *this = other;}

Channel &Channel::operator=(Channel const &src) {
	if (this != &src) {
		this->_name = src._name;
		this->_topic = src._topic;
		this->_clients = src._clients;
	}
	return *this;
}