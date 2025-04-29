#ifndef COMMAND_HPP
# define COMMAND_HPP

# include "./irc.hpp"

class Server;

/*
    Class responsable for processing the commands sent by the client.
	It is responsible for parsing the command and calling the appropriate function.
*/

class Command {

    public:

	Command();
    Command(const Command &other);
    Command &operator=(const Command &other);
	~Command();

    static std::string processCommand(std::string &input, Server& server, User* user);
};

#endif