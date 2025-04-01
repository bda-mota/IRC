#ifndef COMMANDSARGS_HPP
#define COMMANDSARGS_HPP

#include "irc.hpp"

/*
    CommandsArgs é uma classe que possui um map de comandos e funções que executam esses comandos.
    Eu verifico se o comando recebido está no mapa e executo a função correspondente.

*/

class CommandsArgs {

    public:

    typedef std::string (*funcPtr)(const std::vector<std::string> &);

    /* Constructors and Destructor */
    CommandsArgs();
    CommandsArgs(const CommandsArgs& other);
    CommandsArgs& operator=(const CommandsArgs& other);
    ~CommandsArgs();

    /* Método para popular estrutura de comandos */
    static void populateMap();

    /* Método para executar o comando */
    static std::string executeCommand(const std::string &command, const std::vector<std::string> &args);

    // Métodos para execução de comandos
    static std::string user(const std::vector<std::string>& args);
    static std::string nick(const std::vector<std::string>& args);
    static std::string pass(const std::vector<std::string>& args);

    private:

    static std::map<std::string, funcPtr> _messageFunctions;
};

#endif // COMMANDSARGS_HPP