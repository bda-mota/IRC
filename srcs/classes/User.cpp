#include "../../includes/User.hpp"

User::User() {}

User::~User() {}

int User::getFd() const { return _fd; }

std::string User::getIP() const { return _IP; }

std::string User::getUserName() const { return _userName; }

std::string User::getNickName() const { return _nickName; }

void User::setFd(int fd) { _fd = fd; }

void User::setIP(std::string IP) { _IP = IP; }

void User::setUserName(std::string userName) { _userName = userName; }

void User::setNickName(std::string nickName) { _nickName = nickName; }