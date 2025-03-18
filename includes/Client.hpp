#ifndef CLIENT_HPP
# define CLIENT_HPP

# include "./irc.hpp"

class Client {

private:
	int _fd;
	std::string _IP;

public:
	Client();
	~Client();

	int getFd() const;
	std::string getIP() const;

	void setFd(int fd);
	void setIP(std::string IP);
};

#endif