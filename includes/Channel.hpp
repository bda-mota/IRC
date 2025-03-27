#ifndef CHANNEL_HPP
# define CHANNEL_HPP

# include <iostream>
# include <vector>
# include "./Client.hpp"

class Channel {
	private:
		std::string _name;
		std::string _topic;
		std::vector<Client> _clients;

	public:
		//Channel();
		Channel(std::string name);
		Channel(const Channel &other);
		Channel &operator=(Channel const &src);
		~Channel();

		void setName(std::string name);
		void setTopic(std::string topic);
		std::string getName() const;
		std::string getTopic() const;

		void addClient(Client client);
		void removeClient(int fd);
		std::vector<Client> getClients();
};

#endif