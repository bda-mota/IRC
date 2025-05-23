#include "../../includes/Server.hpp"

bool Server::_signal = false;

Server::Server() {
	_serverFd = -1;
	_commandParser = new Command();
}

Server::~Server() {
	std::cout << "Server destructor called." << std::endl;
	if (_commandParser) {
		delete _commandParser;
	}
}

void Server::signalHandler(int signal) {
	(void)signal;
	std::cout << std::endl << "Signal received, closing server." << std::endl;
	Server::_signal = true;
}

void Server::closeFds() {
	std::cout << "Closing all file descriptors..." << std::endl;
	for (size_t i = 0; i < _serverUsers.size(); i++) {
		int fd = _serverUsers[i]->getFd();
		std::cout << "Client: " << _serverUsers[i]->getFd() << " disconnected." << std::endl;
		close(fd);
		delete _serverUsers[i];
	}
	_serverUsers.clear();

	if (_serverFd != -1) {
		std::cout << std::endl << "Server disconnected." << std::endl;
		close(_serverFd);
	}
	_fds.clear();
	clearChannels();
}

void Server::clearUsers(int fd) {
	for (size_t i = 0; i < _serverUsers.size(); i++) {
		if (_serverUsers[i]->getFd() == fd) {
			delete _serverUsers[i];
			_serverUsers.erase(_serverUsers.begin() + i);
			break;
		}
	}

	for (size_t i = 0; i < _fds.size(); i++) {
		if (_fds[i].fd == fd) {
			_fds.erase(_fds.begin() + i);
			break;
		}
	}
}

void Server::clearChannels() {
	for (std::map<std::string, Channel*>::iterator it = _channels.begin(); it != _channels.end(); ++it) {
		delete it->second;
	}
	_channels.clear();
}

void Server::serverSocket() {
	struct sockaddr_in serverAddr;
	struct pollfd newPoll; 
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_addr.s_addr = INADDR_ANY;
	serverAddr.sin_port = htons(this->_port);

	_serverFd = socket(AF_INET, SOCK_STREAM, 0);
	if (_serverFd == -1) {
		throw std::runtime_error("Error: fail to create socket.");
	}

	int en = 1;
	if (setsockopt(_serverFd, SOL_SOCKET, SO_REUSEADDR, &en, sizeof(int)) == -1) { 
		throw std::runtime_error("Error: fail to set option SO_REUSEADDR on socket.");
	}
	if (fcntl(_serverFd, F_SETFL, O_NONBLOCK) == -1) {
		throw std::runtime_error("Error: fail to set option O_NONBLOCK on socket.");
	}
	if (bind(_serverFd, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) == -1) {
		throw std::runtime_error("Error: fail to bind socket.");
	}
	if (listen(_serverFd, SOMAXCONN) == -1) {
		throw std::runtime_error("Error: fail to listen socket.");
	}

	newPoll.fd = _serverFd;
	newPoll.events = POLLIN;
	newPoll.revents = 0;
	_fds.push_back(newPoll);
}

void Server::serverInit(int port, std::string password) {
	this->_port = port;
	this->_password = password;
	serverSocket();

	std::cout << "Server started on port " << this->_port << std::endl;
	std::cout << "Server connected." << std::endl;
	std::cout << "Waiting for clients..." << std::endl;

	while (Server::_signal == false) {

		if (poll(_fds.data(), _fds.size(), 0) == -1 && Server::_signal == false) {
			throw std::runtime_error("Error: fail to poll.");
		}

		for (size_t i = 0; i < _fds.size(); i++) {
			if (_fds[i].revents & POLLIN) {
				if (_fds[i].fd == _serverFd) {
					acceptNewUser();
				} else {
					receiveNewData(_fds[i].fd);
				}
			}
		}
	}
	closeFds();
}

void Server::acceptNewUser() {
	struct sockaddr_in userAddr;
	struct pollfd newPoll;
	socklen_t userAddrSize = sizeof(userAddr);

	int incomingFd = accept(_serverFd, (struct sockaddr *)&userAddr, &userAddrSize);
	if (incomingFd == -1) {
		std::cerr << "Error: fail to accept new client." << std::endl;
		return;
	}

	if (fcntl(incomingFd, F_SETFL, O_NONBLOCK) == -1) {
		throw std::runtime_error("Error: fail to set option O_NONBLOCK on new client.");
	}

	User *newUser = new User();
	newUser->setFd(incomingFd);
	newUser->setIP(inet_ntoa(userAddr.sin_addr));
	newUser->setRegistered(false);
	newUser->setHasNickCommand(false);
	newUser->setHasUserCommand(false);
	newUser->setHasPassCommand(false);
	newUser-> setAuth(false);
	_serverUsers.push_back(newUser);

	newPoll.fd = incomingFd;
	newPoll.events = POLLIN;
	newPoll.revents = 0;

	_fds.push_back(newPoll);

	std::cout << "Client connected: " << newUser->getIP() << std::endl;
}

bool Server::parseReceiveNewData(std::string rawMessage, int fd, User *user) {
	std::istringstream stream(rawMessage);
	std::string line;

	while (std::getline(stream, line)) {
		if (!line.empty() && line[line.length() - 1] == '\r') {
			line = line.substr(0, line.length() - 1);
		}

		if (line.empty())
			continue;

		std::string response = this->_commandParser->processCommand(line, *this, user);

		if (!response.empty()) {
			send(fd, response.c_str(), response.length(), 0);
		}

		if (!userExists(fd)) {
			return false;
		}
	}
	return true;
}

void Server::receiveNewData(int fd) {
	char buff[1024];
	memset(buff, 0, sizeof(buff));

	ssize_t bytes = recv(fd, buff, sizeof(buff) - 1, 0);

	if (bytes <= 0) {
		std::cout << RED << "Client: " << fd << " disconnected." << RESET << std::endl;
		clearUsers(fd);
		close(fd);
		return;
	}

	User* user = NULL;
	for (size_t i = 0; i < _serverUsers.size(); i++) {
		if (_serverUsers[i]->getFd() == fd) {
			user = _serverUsers[i];
			break;
		}
	}

	if (!user)
		return;

	std::string newData(buff, bytes);
	user->appendToBuffer(newData);


    size_t pos;
	while ((pos = user->getReceiveBuffer().find('\n')) != std::string::npos) {
		std::string rawMessage = user->getReceiveBuffer().substr(0, pos + 1);
		user->getReceiveBuffer().erase(0, pos + 1);
	
		if (!parseReceiveNewData(rawMessage, fd, user)) {
			return;
		}
	}
}

void	Server::broadcast(const std::string& message, User* sender) {
	if (!sender) return;

	std::vector<User*>& users = this->getUsers();

	for (std::vector<User*>::iterator it = users.begin(); it != users.end(); ++it) {
		User* target = *it;
		if (target && target->getFd() > 0) {
			send(target->getFd(), message.c_str(), message.length(), 0);
		}
	}
}

const std::map<std::string, Channel*>& Server::getChannels() const { return _channels; }

const std::vector<User*>& Server::getUsers() const { return _serverUsers; }

std::vector<User*>& Server::getUsers() { return _serverUsers; }

std::map<std::string, Channel*>& Server::getChannels() { return _channels; }

int Server::getServerFd() const { return _serverFd; }

std::string Server::getPassword() { return _password; }

bool Server::userExists(int fd) const {
	for (size_t i = 0; i < _serverUsers.size(); i++) {
		if (_serverUsers[i]->getFd() == fd) {
			return true;
		}
	}
	return false;
}