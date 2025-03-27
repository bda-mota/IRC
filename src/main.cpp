#include "../includes/irc.hpp"

int main(int argc, char **argv) {

	if (argc != 3) {
		std::cerr << "Usage: " << argv[0] << " <port> <password>." << std::endl;
		return 1;
	}
	Server ser;

	std::cout << "---- SERVER ----" << std::endl;
	try{
		signal(SIGINT, Server::signalHandler); // ctrl + c
		signal(SIGQUIT, Server::signalHandler); // (ctrl + \)
		ser.serverInit(); //-> initialize the server
	}
	catch(const std::exception& e){
		ser.closeFds(); //-> close the file descriptors
	}
}