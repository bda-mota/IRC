#include "../../includes/Server.hpp"

bool Server::_signal = false; // inicializar a variavel booleana

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
		std::cout << "Client: " << _serverUsers[i]->getFd() << " disconnected." << std::endl;
		close(_serverUsers[i]->getFd());
		delete _serverUsers[i];
	}
	_serverUsers.clear();

	if (_serverFd != -1) {
		std::cout << std::endl << "Server disconnected." << std::endl;
		close(_serverFd);
	}
	clearChannels();
	_fds.clear();
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
	struct sockaddr_in serverAddr; // armazenar informações do endereço ipv4
	struct pollfd newPoll; // estrutura que armazenará o file descriptor do server
	serverAddr.sin_family = AF_INET; //ipv4
	serverAddr.sin_addr.s_addr = INADDR_ANY; // define endereço IP para aceitar qualquer endereço disponível
	serverAddr.sin_port = htons(this->_port); // define a porta do servidor e converte a porta para ser aceita em qualquer sistema

	_serverFd = socket(AF_INET, SOCK_STREAM, 0);
	if (_serverFd == -1) {
		throw std::runtime_error("Error: fail to create socket.");
	}

	int en = 1;
	if (setsockopt(_serverFd, SOL_SOCKET, SO_REUSEADDR, &en, sizeof(int)) == -1) { // definindo opções para o socket | essas opções estão permitindo que o socket reutilize o endereço local
		throw std::runtime_error("Error: fail to set option SO_REUSEADDR on socket.");
	}
	if (fcntl(_serverFd, F_SETFL, O_NONBLOCK) == -1) {
		throw std::runtime_error("Error: fail to set option O_NONBLOCK on socket."); // alterar flag do fd e deixar ele não bloqueante, ele n vai esperar a resposta do cliente
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
	std::cout << GREEN << "Server connected." << RESET << std::endl;
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
	newUser->setUserName("default");
	newUser->setNickName("default");
	newUser->setRealName("default");
	newUser->setRegistered(false);
	newUser->setHasNickCommand(false);
	newUser->setHasUserCommand(false);
	_serverUsers.push_back(newUser);

	newPoll.fd = incomingFd;
	newPoll.events = POLLIN;
	newPoll.revents = 0;

	_fds.push_back(newPoll);

	std::cout << GREEN << "Client connected: " << newUser->getIP() << RESET << std::endl;
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

	std::string rawMessage(buff);
	std::cout << "Buff: " << buff << std::endl;

	std::string response = this->_commandParser->processCommand(rawMessage, *this, user);

	// Só responde se realmente tiver algo pra responder (tipo um erro)
	if (!response.empty())
		send(fd, response.c_str(), response.length(), 0);
}

void	Server::broadcast(const std::string& message, User* sender) {
	if (!sender) {
		return;
	}

	std::vector<User*>& users = this->getUsers();

	for (std::vector<User*>::iterator it = users.begin(); it != users.end(); ++it) {
		User* target = *it;
		if (target && target->getFd() > 0 && target->getFd() != sender->getFd()) {
			send(target->getFd(), message.c_str(), message.length(), 0);
		}
	}
}

// GETTERS
const std::map<std::string, Channel*>& Server::getChannels() const { return _channels; }

const std::vector<User*>& Server::getUsers() const { return _serverUsers; }

std::vector<User*>& Server::getUsers() { return _serverUsers; }

std::map<std::string, Channel*>& Server::getChannels() { return _channels; }

int Server::getServerFd() const { return _serverFd; }
