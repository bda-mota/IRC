#include "../includes/irc.hpp"

int main(int argc, char **argv) {
	if (argc != 3) {
		std::cerr << "Usage: " << argv[0] << " <port> <password>." << std::endl;
		return 1;
	}

	std::cout << "---- SERVER ----" << std::endl;
	try{
    std::string portStr = argv[1];
    std::string password = argv[2];
	  Server ser(portStr, password);
		signal(SIGINT, Server::signalHandler); // ctrl + c
		signal(SIGQUIT, Server::signalHandler); // (ctrl + \)
		ser.serverInit(); //-> initialize the server
	}
	catch(const std::exception& e){
    std::cerr << "Error: " << e.what() << std::endl;
		return 1;
	}

  return 0;
}
