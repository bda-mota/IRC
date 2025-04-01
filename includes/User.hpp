#ifndef CLIENT_HPP
# define CLIENT_HPP

# include "./irc.hpp"

class User {

	private:
		int _fd;
		std::string _IP;
		std::string _userName;
		std::string _nickName;

	public:
		User();
		~User();

		int getFd() const;
		std::string getIP() const;
		std::string getUserName() const;
		std::string getNickName() const;

		void setFd(int fd);
		void setIP(std::string IP);
		void setUserName(std::string userName);
		void setNickName(std::string nickName);
};

#endif