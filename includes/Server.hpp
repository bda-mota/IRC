#ifndef SERVER_HPP
# define SERVER_HPP

# include "./irc.hpp"

<<<<<<< HEAD
class Client;
class Command;

class Server {

private:

	//TODO:deixar esta estrutura aqui?
	Command *_commandParser;

	std::string _password;
	int _port;
	int _serverFd; // fd que será atribuído após criar o socket do server, inicialmente será -1
	static bool _signal; // variável que será usada para verificar se o server foi encerrado
	std::vector<Client> clients;
	std::vector<struct pollfd> fds; // vector que armazenará os file descriptors dos clientes
=======
class User;
class Channel;

class Server {

	private:
		int _port;
		int _serverFd; // fd que será atribuído após criar o socket do server, inicialmente será -1
		static bool _signal; // variável que será usada para verificar se o server foi encerrado
		std::string _password;
		std::vector<User> users; // vector que armazenará os clientes
		std::vector<struct pollfd> fds; // vector que armazenará os file descriptors dos clientes
		std::map<std::string, Channel> channels; // onde ficarão os canais
>>>>>>> 16b594c92be1a07eb3cd45df5c45e4a6fae5c02e

	public:
		Server();
		~Server();

		void serverInit(); //iniciar o server, temos que mandar a porta e o password p /ele 
		void serverSocket(); // criar o socket, fazer o bind, listen
		void acceptNewUser(); // aceitar um novo cliente
		void receiveNewData(int fd); // receber novos dados dos clientes

		static void signalHandler(int signal); // função que será chamada quando o server for encerrado

		void closeFds(); // fechar todos os file descriptors
		void clearUsers(int fd); // limpar todos os clientes que estão no vector do server
		
		std::map<std::string, Channel>& getChannels(); // da para modificar os channels do server
		std::vector<User>& getUsers(); // da para modificar os users do server
};

#endif