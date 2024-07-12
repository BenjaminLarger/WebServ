#include "../inc/POST.hpp"
#include "../inc/Dependencies.hpp"

void	POST::extractBody() {
// Read body
	char	*buffer = new char[contentLength + 1];

	requestStream.read(buffer, contentLength);
	buffer[contentLength] = '\0';
	body = buffer;
	delete[] buffer;
	std::cout << "body: " << body << std::endl;
}

void	POST::extractHeaders(std::string &clientInput) {
	std::string	line;
	this->requestStream.str(clientInput);

	while (std::getline(requestStream, line) && line != "\r") {
		size_t colonPos = line.find(":");
		if (colonPos != std::string::npos) {
			std::string headerName = line.substr(0, colonPos);
			std::string headerValue = line.substr(colonPos + 1);
			// Trim whitespace
			headerValue.erase(0, headerValue.find_first_not_of(" \t"));
			headerValue.erase(headerValue.find_last_not_of(" \t") + 1);
			this->headers[headerName] = headerValue;
			if (headerName == "Content-Length")
				contentLength = std::atoi(headerValue.c_str());
			else if (headerName == "Content-Type")
				contentType = headerValue;
			else if (headerName == "Host")
				host = headerValue;
		}
	}

	std::cout << "content-type: " << this->contentType << std::endl;
	std::cout << "content-length: "<< this->contentLength << std::endl;
	std::cout << "host: " << this->host << std::endl;
}

void	POST::extractFirstLine(std::string &clientInput) {
	std::string	line;

	this->requestStream.str(clientInput);//This is going to be in the constructor when clientInput is passed as parameter
	this->requestStream >> line;
	this->requestStream >> this->pathToRessource;
	this->requestStream >> this->HTTPversion;
	std::cout  << "path-to-resource: " << this->pathToRessource << RESET << std::endl;
	std::cout  << "HTTP: " << this->HTTPversion << RESET << std::endl;
	requestStream.clear();
}

//We extract all the content of a POST request
POST::POST(Webserv _server, int serverFD, int clientFD, std::string &clientInput)
	: contentLength(0) {

	(void)_server;
	(void)serverFD;
	(void)clientFD;
	std::string line;

	// Read headers
	clientInput = "POST /submit-form HTTP/1.1\r\n"
					"Host: localhost:8080\r\n"
					"Content-Type: application/x-www-form-urlencoded\r\n"
					"Content-Length: 16\r\n"
					"\r\n"
					"name=John&age=30";
	extractFirstLine(clientInput);
	extractHeaders(clientInput);
	extractBody();
}

POST::~POST(void) {

}