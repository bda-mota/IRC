#ifndef CHANNEL_HPP
# define CHANNEL_HPP

# include <iostream>
# include "./irc.hpp"

class Channel {
	private:
		std::string _name;
		std::string _topic;
		std::vector<User*> _channelUsers;

	public:
		Channel();
		Channel(std::string name);
		Channel(Channel const& other);
		Channel &operator=(Channel const& other);
		~Channel();

		void setName(std::string name);
		void setTopic(std::string topic);
		const std::string& getName() const;
		const std::string& getTopic() const;
		std::vector<User*>& getUsers();

		void	addUser(User* user);
		void	removeUser(int fd);
		void	broadcast(const std::string& message, User* sender);
};

#endif