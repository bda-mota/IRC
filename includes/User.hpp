#ifndef USER_HPP
# define USER_HPP

# include "./irc.hpp"

class User {

	private:
		int _fd;
		std::string _IP;
		std::string _userName;
		std::string _nickName;
		std::string _realName; //verificar se é necessário

	public:
		User();
		User(int fd, std::string ip, std::string userName, std::string nickName);
		virtual ~User();

		int getFd() const;
		const std::string& getIP() const;
		const std::string& getUserName() const;
		const std::string& getNickName() const;
		const std::string& getRealName() const;
		
		void setFd(int fd);
		void setIP(std::string const& IP);
		void setUserName(std::string const& userName);
		void setNickName(std::string const& nickName);
		void setRealName(std::string const& realName);
};

#endif