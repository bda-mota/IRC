#ifndef COMMANDPARSE_HPP
# define COMMANDPARSE_HPP

# include "./irc.hpp"

class CommandParse {

private:

    // atributos

public:

    /* Constructors and Destructor */
	CommandParse();
    CommandParse(const CommandParse &other);
    CommandParse &operator=(const CommandParse &other);
	~CommandParse();

    // métodos, getters e setters

    /* Método responsável pelo processamento do comando enviado pelo cliente*/
    std::string processCommand(std::string &input);
};

#endif