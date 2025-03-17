#include "../includes/Server.hpp" 

bool Server::_signal = false; // inicializar a variavel booleana

Server::Server() {_serverFd = -1;}

Server::~Server() {}

void Server::signalHandler(int signal) {
	(void)signal;
	std::cout << std::endl << "Signal received, closing server." << std::endl;
	Server::_signal = true; // é mais seguro e rápido do que usar o setSignal
}

void Server::closeFds() {
	for (size_t i = 0; i < clients.size(); i++) {
		std::cout << "Client: " << clients[i].getFd() << " disconnected." << std::endl;
		close(clients[i].getFd());
	}
	if (_serverFd != -1) {
		std::cout << std::endl << "Server disconnected." << std::endl;
		close(_serverFd);
	}
}


void Server::clearClients(int fd) {
	for (size_t i = 0; i < fds.size(); i++) {
		if (fds[i].fd == fd) {
			fds.erase(fds.begin() + i);
			break;
		}
	}
	for (size_t i = 0; i < clients.size(); i++) {
		if (clients[i].getFd() == fd) {
			clients.erase(clients.begin() + i);
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
}