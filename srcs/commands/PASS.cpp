#include "../../includes/irc.hpp"

std::string CommandsArgs::pass(const std::vector<std::string>& args, Server& server, User* user) {
  std::cout << "Received PASS command from fd " << user->getFd() << std::endl;

  if (user->isAuth()) {
      std::cout << "Client " << user->getFd() << " attempted to resend PASS after registration." << std::endl;
      return ERR_ALREADYREGISTERED(user->getNickName());
  }

  if (args.size() < 1) {
      std::cout << "Client " << user->getFd() << " sent PASS with missing parameter." << std::endl;
      return ERR_NEEDMOREPARAMS("PASS", "Not enough parameters");
  }

  if (args[0] != server.getPassword()) {
      std::cout << "Client " << user->getFd() << " provided wrong password." << std::endl;
      return ERR_PASSWDMISMATCH();
  }

  user->setAuth(true);
  std::cout << "Client " << user->getFd() << " authenticated successfully with PASS." << std::endl;
  return "";
}
