#ifndef CHANNEL_HPP
# define CHANNEL_HPP

# include <iostream>
# include "./irc.hpp"

class Channel {
	private:
		std::string        _name;
		std::string        _topic;
		bool 		           _inviteOnly;
    bool               _topicRestricted;
    int                _userLimit;
    std::string        _channelKey;
		std::vector<User*> _channelUsers;
		std::set<User*>    _operators;

	public:
		Channel();
		Channel(std::string name);
		Channel(Channel const& other);
		Channel &operator=(Channel const& other);
		~Channel();

		void	              setName(std::string name);
		std::string       	setTopic(std::string topic, User* user);
		const	              std::string& getName() const;
		const	              std::string& getTopic() const;
		std::vector<User*>& getUsers();

		// Users
		bool	      isUserInChannel(User* user) const;
		std::string	addUser(User* user, const std::string& key = "");
		void	      removeUser(User *user);
		void	      broadcast(const std::string& message, User* sender);

		// Operators
		void	addOperator(User* user);
		void	removeOperator(User* user);
		bool	isOperator(User* user) const;

		// Invite only
		bool	isInviteOnly() const;
		void	setInviteOnly(bool inviteOnly);

    // Topic Restricted
		bool	isTopicRestricted() const;
		void	setTopicRestricted(bool topicRestricted);

    // User Limit
    int         getUserLimit() const;
    std::string setUserLimit(int limit);
    bool        isFull() const;

    // Channel Key
    std::string getChannelKey() const;
    void        setChannelKey(const std::string& key);
    bool        hasKey() const;

    // User by Nick
    User* getUserByNick(const std::string& nickname) const;
};

#endif
