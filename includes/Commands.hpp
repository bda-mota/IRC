#ifndef COMMANDS_HPP
#define COMMANDS_HPP

#include <map>
#include <string>

class Commands {

    public:

    typedef funcPtr = std::string (*)(const CommandArgs &);

    /* Constructors and Destructor */
    Commands();
    Commands(const Commands& other);
    Commands& operator=(const Commands& other);
    ~Commands();

    /* Método para popular estrutura de comandos */
    static void populateMap();

    private:

    static std::map<std::string, funcPtr> _messageFunctions;

};

#endif // COMMANDS_HPP