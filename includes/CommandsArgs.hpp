#ifndef COMMANDSARGS_HPP
#define COMMANDSARGS_HPP

#include "irc.hpp"

class CommandsArgs {

    public:

    typedef std::string (*funcPtr)(const CommandsArgs &);

    /* Constructors and Destructor */
    CommandsArgs();
    CommandsArgs(const CommandsArgs& other);
    CommandsArgs& operator=(const CommandsArgs& other);
    ~CommandsArgs();

    /* Método para popular estrutura de comandos */
    static void populateMap();

    private:

    static std::map<std::string, funcPtr> _messageFunctions;

};

std::string user(const CommandsArgs &args);

#endif // COMMANDSARGS_HPP