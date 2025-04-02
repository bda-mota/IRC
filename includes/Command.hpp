#ifndef COMMAND_HPP
# define COMMAND_HPP

# include "./irc.hpp"

class Server;

/*
    Classe responsável por processar os comandos enviados pelo cliente.
    Aqui eu pego a mensagem enviada pelo cliente, separo o comando e os argumentos, armazeno o comando em _command e envio os argumentos para CommandsArgs.
*/

class Command {

    public:

    /* Constructors and Destructor */
	Command();
    Command(const Command &other);
    Command &operator=(const Command &other);
	~Command();

    // métodos, getters e setters

    /* Método responsável pelo processamento do comando enviado pelo cliente */
    static std::string processCommand(std::string &input, Server& server, User* user);
};

#endif