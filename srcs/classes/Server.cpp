#include "../../includes/Server.hpp" 

bool Server::_signal = false; // inicializar a variavel booleana

Server::Server() {
	_serverFd = -1;
	_commandParser = new Command();
}

Server::~Server() {}

void Server::signalHandler(int signal) {
	(void)signal;
	std::cout << std::endl << "Signal received, closing server." << std::endl;
	Server::_signal = true; // é mais seguro e rápido do que usar o setSignal
}

void Server::closeFds() {
	for (size_t i = 0; i < users.size(); i++) {
		std::cout << "Client: " << users[i].getFd() << " disconnected." << std::endl;
		close(users[i].getFd());
	}
	if (_serverFd != -1) {
		std::cout << std::endl << "Server disconnected." << std::endl;
		close(_serverFd);
	}
}

void Server::clearUsers(int fd) {
	for (size_t i = 0; i < fds.size(); i++) {
		if (fds[i].fd == fd) {
			fds.erase(fds.begin() + i);
			break;
		}
	}
	for (size_t i = 0; i < users.size(); i++) {
		if (users[i].getFd() == fd) {
			users.erase(users.begin() + i);
			break;
		}
	}
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
	fds.push_back(newPoll);
}

void Server::serverInit() {
	this->_port = 4444;
	serverSocket();

	std::cout << "Server started on port " << this->_port << std::endl;
	std::cout << GREEN << "Server connected." << RESET << std::endl;
	std::cout << "Waiting for clients..." << std::endl;

	while (Server::_signal == false) {
		
		if (poll(fds.data(), fds.size(), 0) == -1 && Server::_signal == false) {
			throw std::runtime_error("Error: fail to poll.");
		}

		for (size_t i = 0; i < fds.size(); i++) {
			if (fds[i].revents & POLLIN) {
				if (fds[i].fd == _serverFd) {
					acceptNewUser();
				} else {
					receiveNewData(fds[i].fd);
				}
			}
		}
	}
	closeFds();
}

void Server::acceptNewUser() {
	User newUser;
	struct sockaddr_in userAddr;
	struct pollfd newPoll;
	socklen_t userAddrSize = sizeof(userAddr);

	int incomingFd = accept(_serverFd, (struct sockaddr *)&userAddr, &userAddrSize);
	if (incomingFd == -1) {
		throw std::runtime_error("Error: fail to accept new client.");
	}

	if (fcntl(incomingFd, F_SETFL, O_NONBLOCK) == -1) {
		throw std::runtime_error("Error: fail to set option O_NONBLOCK on new client.");
	}

	newPoll.fd = incomingFd;
	newPoll.events = POLLIN;
	newPoll.revents = 0;

	newUser.setFd(incomingFd);
	newUser.setIP(inet_ntoa(userAddr.sin_addr));
	users.push_back(newUser);
	fds.push_back(newPoll);

	std::cout << GREEN << "Client connected: " << newUser.getIP() << RESET << std::endl;
}

void Server::receiveNewData(int fd) {
	char buff[1024];
	memset(buff, 0, sizeof(buff));

	ssize_t bytes = recv(fd, buff, sizeof(buff) - 1, 0);

	if (bytes <= 0) {
		std::cout << RED << "Client: " << fd << " disconnected." << RESET << std::endl;
		clearUsers(fd);
		close(fd);
	} else {
		std::string rawMessage(buff);

		std::string response = this->_commandParser->processCommand(rawMessage, *this, &users[fd]);

		send(fd, buff, bytes, 0);
	}
}

std::map<std::string, Channel>& Server::getChannels() { return channels; }

std::vector<User>& Server::getUsers() { return users; }

int Server::getServerFd() const { return _serverFd; }

void	Server::broadcast(const std::string& message, User* sender) {
	std::vector<User>& users = this->getUsers();

	for (std::vector<User>::iterator it = users.begin(); it != users.end(); ++it) {
		if (it->getFd() != sender->getFd()) {
			send(it->getFd(), message.c_str(), message.length(), 0);
		}
	}
}