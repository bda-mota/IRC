#ifndef SERVER_HPP
# define SERVER_HPP

# include "./irc.hpp"

class Command;
class Channel;
class User;

class Server {

	private:
		Command *_commandParser;
		int _port;
		int _serverFd; // fd que será atribuído após criar o socket do server, inicialmente será -1
		static bool _signal; // variável que será usada para verificar se o server foi encerrado
		std::string _password;
		std::vector<User*> _serverUsers; // vector que armazenará os clientes
		std::vector<struct pollfd> _fds; // vector que armazenará os file descriptors dos clientes
		std::map<std::string, Channel*> _channels; // onde ficarão os canais

	public:
		Server();
		~Server();

		void serverInit(); //iniciar o server, temos que mandar a porta e o password p /ele 
		void serverSocket(); // criar o socket, fazer o bind, listen
		void acceptNewUser(); // aceitar um novo cliente
		void receiveNewData(int fd); // receber novos dados dos clientes
		static void signalHandler(int signal); // função que será chamada quando o server for encerrado
		void broadcast(const std::string& message, User* sender); // enviar mensagem para todos os clientes

		// CLEARING
		void closeFds(); // fechar todos os file descriptors
		void clearUsers(int fd); // limpar todos os clientes que estão no vector do server
		void clearChannels();
		
		//GETTERS
		std::map<std::string, Channel*>& getChannels(); 
		std::vector<User*>& getUsers(); 
		const std::map<std::string, Channel*>& getChannels() const; 
		const std::vector<User*>& getUsers() const; 

		int getServerFd() const;


};

#endif