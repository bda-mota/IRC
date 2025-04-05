#include "../../includes/User.hpp"

User::User() : _fd(-1), _IP(""), _userName(""), _nickName("") {};

User::User(int fd, std::string ip, std::string userName, std::string nickName) : _fd(fd), _IP(ip), _userName(userName),  _nickName(nickName){}

User::~User() {}

int User::getFd() const { return _fd; }

const std::string& User::getIP() const { return _IP; }

const std::string& User::getUserName() const { return _userName; }

const std::string& User::getNickName() const { return _nickName; }

void User::setFd(int fd) { _fd = fd; }

void User::setIP(std::string const& IP) { _IP = IP; }

void User::setUserName(std::string const& userName) { _userName = userName; }

void User::setNickName(std::string const& nickName) { _nickName = nickName; }

void User::setRealName(std::string const& realName) { _realName = realName; }