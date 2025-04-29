#include "../includes/irc.hpp"
#include "../includes/utils.hpp"

int main(int argc, char **argv) {

	if (argc != 3) {
		std::cerr << "Usage: " << argv[0] << " <port> <password>." << std::endl;
		return 1;
	}
	int port;
	std::string portStr = argv[1];
	std::string password = argv[2];

	try {
		port = Utils::validateInputs(portStr, password);
	}
	catch (const std::exception& e) {
		std::cerr << "Error: " << e.what() << std::endl;
		return 1;
	}
	Server ser;

	std::cout << "---- SERVER ----" << std::endl;
	try{

		CommandsArgs::populateMap();
		signal(SIGINT, Server::signalHandler);
		signal(SIGQUIT, Server::signalHandler);
		ser.serverInit(port, password);
	}
	catch(const std::exception& e){
		ser.closeFds();
	}
}
