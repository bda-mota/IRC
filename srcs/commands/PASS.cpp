#include "../../includes/irc.hpp"

std::string CommandsArgs::pass(const std::vector<std::string>& args, Server& server, User* user) {
  std::cout << "Received PASS command from fd " << user->getFd() << std::endl;

	if (user->isAuth()) {
		std::cout << "Client " << user->getFd() << " attempted to resend PASS after registration." << std::endl;
		return user->getNickName() + " :You may not register again\r\n";
	}

	if (args.size() < 1) {
		std::cout << "Client " << user->getFd() << " sent PASS with missing parameter." << std::endl;
		return "PASS :Not enough parameters\r\n";
	}
	server.setPassword(args[0]);
	user->setAuth(true);
	std::cout << "Client " << user->getFd() << " authenticated successfully with PASS." << std::endl;
	return "";
}
