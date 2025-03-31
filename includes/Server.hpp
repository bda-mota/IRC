#ifndef SERVER_HPP
# define SERVER_HPP

# include "./irc.hpp"

class Client;
class CommandParse;

class Server {

private:

	//TODO:deixar esta estrutura aqui?
	CommandParse *_commandParser;

	std::string _password;
	int _port;
	int _serverFd; // fd que será atribuído após criar o socket do server, inicialmente será -1
	static bool _signal; // variável que será usada para verificar se o server foi encerrado
	std::vector<Client> clients;
	std::vector<struct pollfd> fds; // vector que armazenará os file descriptors dos clientes

public:
	Server();
	~Server();

	void serverInit(); //iniciar o server, temos que mandar a porta e o password p /ele 
	void serverSocket(); // criar o socket, fazer o bind, listen
	void acceptNewClient(); // aceitar um novo cliente
	void receiveNewData(int fd); // receber novos dados dos clientes

	static void signalHandler(int signal); // função que será chamada quando o server for encerrado

	void closeFds(); // fechar todos os file descriptors
	void clearClients(int fd); // limpar todos os clientes que estão no vector do server 
};

#endif