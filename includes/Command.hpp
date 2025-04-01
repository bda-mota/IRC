#ifndef COMMAND_HPP
# define COMMAND_HPP

# include "./irc.hpp"

class Command {

    private:

    std::string _command; // comando

    public:

    /* Constructors and Destructor */
	Command();
    Command(const Command &other);
    Command &operator=(const Command &other);
	~Command();

    // métodos, getters e setters

    /* Método responsável pelo processamento do comando enviado pelo cliente */
    std::string processCommand(std::string &input);
};

#endif