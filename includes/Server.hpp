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
		int _serverFd;
		static bool _signal;
		std::string _password;
		std::vector<User*> _serverUsers;
		std::vector<struct pollfd> _fds;
		std::map<std::string, Channel*> _channels;

	public:
		Server();
		~Server();

		void	serverInit(int port, std::string password);
		void	serverSocket();
		void	acceptNewUser();
		void	receiveNewData(int fd);
		void	broadcast(const std::string& message, User* sender);
		static void signalHandler(int signal);

		// Clearing
		void	closeFds();
		void	clearUsers(int fd);
		void	clearChannels();

		// Getters
		std::map<std::string, Channel*>& getChannels();
		std::vector<User*>& getUsers();
		const std::map<std::string, Channel*>& getChannels() const;
		const std::vector<User*>& getUsers() const;
		int	getServerFd() const;
    	std::string	getPassword();

    	bool	parseReceiveNewData(std::string rawMessage, int fd, User *user);
		bool	userExists(int fd) const;
};

#endif
