#include "../includes/CommandParse.hpp"

/* CONSTRUCTORS and DESTRUCTOR */

CommandParse::CommandParse() {}

CommandParse::CommandParse(const CommandParse &other) {
    *this = other;
}

CommandParse &CommandParse::operator=(const CommandParse &other) {
    if (this != &other) {
        // adicionar atributos aqui
    }
    return *this;
}

CommandParse::~CommandParse() {}
