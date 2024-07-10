#include "../inc/POST.hpp"

POST::POST(Webserv _server, int serverFD, int clientFD, std::string &clientInput) {


	(void)_server;
	(void)serverFD;
	(void)clientFD;

	std::istringstream isLine(clientInput);
	std::string key;
	std::cout << clientInput << std::endl;
}

POST::~POST(void) {

}