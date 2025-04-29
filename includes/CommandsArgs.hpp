#ifndef COMMANDSARGS_HPP
#define COMMANDSARGS_HPP

#include "irc.hpp"
#include "Server.hpp"

class Server;
class User;

class CommandsArgs {

    public:

    typedef std::string (*funcPtr)(const std::vector<std::string>&, Server&, User*);

    CommandsArgs();
    CommandsArgs(const CommandsArgs& other);
    CommandsArgs& operator=(const CommandsArgs& other);
    ~CommandsArgs();

    static void populateMap();

    static std::string executeCommand(const std::string& command, const std::vector<std::string>& args, Server& server, User* user);

    static std::string user(const std::vector<std::string>& args, Server& server, User* user);
    static std::string nick(const std::vector<std::string>& args, Server& server, User* user);
    static std::string pass(const std::vector<std::string>& args, Server& server, User* user);
    static std::string quit(const std::vector<std::string>& args, Server& server, User* user);
    static std::string join(const std::vector<std::string>& args, Server& server, User* user);
    static std::string listc(const std::vector<std::string>& args, Server& server, User* user);
    static std::string privmsg(const std::vector<std::string>& args, Server& server, User* sender);
    static std::string kick(const std::vector<std::string>& args, Server& server, User* user);
    static std::string part(const std::vector<std::string>& args, Server& server, User* user);
    static std::string who(const std::vector<std::string>& args, Server& server, User* user);
    static std::string mode(const std::vector<std::string>& args, Server& server, User* user);
    static std::string topic(const std::vector<std::string>& args, Server& server, User* user);
    static std::string invite(const std::vector<std::string>& args, Server& server, User* user);
    static std::string cap(const std::vector<std::string>& args, Server& server, User* user);

    private:

    static std::map<std::string, funcPtr> _messageFunctions;

};

#endif // COMMANDSARGS_HPP
