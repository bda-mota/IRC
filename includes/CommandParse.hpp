#ifndef COMMANDPARSE_HPP
# define COMMANDPARSE_HPP

# include "./irc.hpp"

class CommandParse {

public:

    typedef std::string (*funcPointer)(const std::string, std::vector<std::string>); // TODO: mudar para CommandArgs depois de criar

    /* Constructors and Destructor */
	CommandParse();
    CommandParse(const CommandParse &other);
    CommandParse &operator=(const CommandParse &other);
	~CommandParse();

    // métodos, getters e setters

    /* Método responsável pelo processamento do comando enviado pelo cliente*/
    std::string processCommand(std::string &input);
    
    /* Método responsável por adicionar um comando ao mapa de comandos */
    void populateCommandsMap();

    /* Métodos responsáveis por executar os comandos */
    static std::string user(const std::string& cmd, std::vector<std::string> args);
    static std::string nick(const std::string& cmd, std::vector<std::string> args);
    static std::string pass(const std::string& cmd, std::vector<std::string> args);
    static std::string cap(const std::string& cmd, std::vector<std::string> args);
    static std::string quit(const std::string& cmd, std::vector<std::string> args);
    static std::string join(const std::string& cmd, std::vector<std::string> args);
    static std::string listc(const std::string& cmd, std::vector<std::string> args);
    static std::string privmsg(const std::string& cmd, std::vector<std::string> args);
    static std::string kick(const std::string& cmd, std::vector<std::string> args);
    static std::string part(const std::string& cmd, std::vector<std::string> args);
    static std::string who(const std::string& cmd, std::vector<std::string> args);
    static std::string mode(const std::string& cmd, std::vector<std::string> args);
    static std::string topic(const std::string& cmd, std::vector<std::string> args);
    static std::string invite(const std::string& cmd, std::vector<std::string> args);

    private:

    //std::map<const std::string&, funcPointer> _commandMap; // mapa de comandos
    std::map<std::string, std::string (*)(const std::string&, std::vector<std::string>)> _commandMap;
    std::string _command; // comando

};

#endif