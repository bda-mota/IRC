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
		std::string _realName;
		std::string _hostName;
		std::string _serverName;
		std::vector<Channel*> _joinedChannels;
    	
		bool _auth;
		bool _hasUserCommand;
		bool _hasNickCommand;
		bool _hasPassCommand;
		bool _hasRegistered;

	public:
		User();
		User(int fd, std::string ip, std::string userName, std::string nickName);
		virtual ~User();

		int	getFd() const;
		const std::string& getIP() const;
		const std::string& getHostName() const;
		const std::string& getServerName() const;
		const std::string& getUserName() const;
		const std::string& getNickName() const;
		const std::string& getRealName() const;
		
		const std::vector<Channel*>& getJoinedChannels() const;
		std::vector<Channel*>& getJoinedChannels();
		
    	bool	isAuth() const;
		bool	getHasUserCommand() const;
		bool	getHasNickCommand() const;
		bool	getRegistered() const;
		bool	getHasPassCommand() const;

		void	setFd(int fd);
		void	setIP(std::string const& IP);
		void	setUserName(std::string const& userName);
		void	setNickName(std::string const& nickName);
		void	setRealName(std::string const& realName);
		void	setHostName(std::string const& hostname);
		void	setServerName(std::string const& servername);
		
    	void	setAuth(bool auth);
		void	setRegistered(bool registered);
		void	setHasUserCommand(bool hasUserCommand);
		void	setHasNickCommand(bool hasNickCommand);
		void	setHasPassCommand(bool hasPassCommand);
		
		bool	isInChannel(Channel* channel) const;
		void	removeChannel(Channel* channel);
		void	joinChannel(Channel* channel);
};

#endif
