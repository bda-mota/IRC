#include "../includes/Server.hpp"

bool Server::_signal = false; // inicializar a variavel booleana

Server::Server(std::string portStr, std::string password) {
    if (!checkPort(portStr)) {
        throw std::invalid_argument("Invalid port: " + portStr + ". Please use a numeric port between 1024 and 49151.");
    }
    _port = std::stoi(portStr);
    _password = password;
    _serverFd = -1;
}

Server::~Server() {
  closeFds();
}

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
          try
          {
            acceptNewClient();
          }
          catch(const std::exception& e)
          {
            std::cerr << e.what() << '\n';
          }
				} else {
					receiveNewData(fds[i].fd);
				}
			}
		}
	}
	closeFds();
}

void Server::acceptNewClient() {
	Client newClient;
	struct sockaddr_in clientAddr;
	struct pollfd newPoll;
	socklen_t clientAddrSize = sizeof(clientAddr);

	int incomingFd = accept(_serverFd, (struct sockaddr *)&clientAddr, &clientAddrSize);
	if (incomingFd == -1) {
		throw std::runtime_error("Error: fail to accept new client.");
	}

	if (fcntl(incomingFd, F_SETFL, O_NONBLOCK) == -1) {
		throw std::runtime_error("Error: fail to set option O_NONBLOCK on new client.");
	}

  // Read password
  char buff[1024];
  memset(buff, 0, sizeof(buff));
  ssize_t bytes = recv(incomingFd, buff, sizeof(buff) - 1, 0);

  if (bytes == -1) {
      std::cerr << "recv() failed! errno: " << errno << " (" << strerror(errno) << ")" << std::endl;
      std::string errorMsg = "Connection closed due to error or empty password.\n";
      send(incomingFd, errorMsg.c_str(), errorMsg.length(), 0);
      close(incomingFd);
      throw std::runtime_error("Error: fail to authenticate new client.");
  }


  // Convert to string and remove trailing newline (if present)
  std::string receivedPassword(buff);
  receivedPassword.erase(std::remove(receivedPassword.begin(), receivedPassword.end(), '\n'), receivedPassword.end());
  receivedPassword.erase(std::remove(receivedPassword.begin(), receivedPassword.end(), '\r'), receivedPassword.end());

  // Compare password
  if (receivedPassword != _password) {
      std::string errorMsg = "Incorrect password. Connection closed.\n";
      send(incomingFd, errorMsg.c_str(), errorMsg.length(), 0);
      close(incomingFd);
      std::cout << RED << "Client failed authentication: " << inet_ntoa(clientAddr.sin_addr) << RESET << std::endl;
      throw std::runtime_error("Error: fail to authenticate new client.");
  }

	newPoll.fd = incomingFd;
	newPoll.events = POLLIN;
	newPoll.revents = 0;

	newClient.setFd(incomingFd);
	newClient.setIP(inet_ntoa(clientAddr.sin_addr));
	clients.push_back(newClient);
	fds.push_back(newPoll);

	std::cout << GREEN << "Client connected: " << newClient.getIP() << RESET << std::endl;
}

void Server::receiveNewData(int fd) {
	char buff[1024];
	memset(buff, 0, sizeof(buff));

	ssize_t bytes = recv(fd, buff, sizeof(buff) - 1, 0);

	if (bytes <= 0) {
		std::cout << RED << "Client: " << fd << " disconnected." << RESET << std::endl;
		clearClients(fd);
		close(fd);
	} else {
		std::cout << "Client: " << fd << " sent: " << buff << std::endl;
		send(fd, buff, bytes, 0);
	}
}

bool Server::checkPort(std::string portStr) {
  //corigir loop
  if (!std::all_of(portStr.begin(), portStr.end(), ::isdigit)) return false;
  int port = std::stoi(portStr);
  // checar se a porta é válida
  // 0 - 1023 são portas reservadas para o sistema, deve ser evitado a não ser que seja um superuser
  // 1024 - 49151 são portas registradas, podem ser usadas por aplicações
  // 49152 - 65535 são portas dinâmicas, podem ser usadas por aplicações mas são temporárias usada por clientes. Não dever ser usada por servidores.
  if (port < 1024 || port > 49151) {
    return false;
  }
  return true;
}
