#ifndef USER_HPP
# define USER_HPP

# include "./irc.hpp"

class Channel;

class User {

	private:
		int _fd;
		std::string _IP;
		std::string _userName;
		std::string _nickName;
		std::string _realName; //verificar se é necessário
		std::vector<Channel*> _joinedChannels; 

	public:
		User();
		User(int fd, std::string ip, std::string userName, std::string nickName);
		virtual ~User();

		int	getFd() const;
		const std::string& getIP() const;
		const std::string& getUserName() const;
		const std::string& getNickName() const;
		const std::string& getRealName() const;
		const std::vector<Channel*>& getJoinedChannels() const;
		std::vector<Channel*>& getJoinedChannels();
		
		void	setFd(int fd);
		void	setIP(std::string const& IP);
		void	setUserName(std::string const& userName);
		void	setNickName(std::string const& nickName);
		void	setRealName(std::string const& realName);

		void	joinChannel(Channel* channel);
		bool	isInChannel(Channel* channel) const;
};

#endif