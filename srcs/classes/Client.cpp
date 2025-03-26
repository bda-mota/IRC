#include "../../includes/Client.hpp"

Client::Client() {}

Client::~Client() {}

int Client::getFd() const { return _fd; }

std::string Client::getIP() const { return _IP; }

void Client::setFd(int fd) { _fd = fd; }

void Client::setIP(std::string IP) { _IP = IP; }