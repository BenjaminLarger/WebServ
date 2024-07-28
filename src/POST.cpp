#include "../inc/POST.hpp"
#include "../inc/Dependencies.hpp"

//Send form data to a URL and get a response back

void	POST::extractBody(int clientFD) {
// Read body
	if (contentLength > 0)
	{
		char	*buffer = new char[contentLength + 1];

		//Reads the content of the body from the actual position of the stream
		requestStream.read(buffer, contentLength);
		buffer[contentLength] = '\0';
		//Converts the buffer to a string to make it easier to manipulate
		body = buffer;
		delete[] buffer;
		std::cout << "body: " << body << std::endl;
		// 200 = code for success received, OK = brief description of the status, text/plain = type of the response
		std::string	response = "HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\n\r\n";
		response += "POST data received.\n";
		send(clientFD, response.c_str(), response.size(), 0);
	}
	else
		sendErrorResponse(clientFD, "411", "Length required");
	close(clientFD);
}

void	POST::extractHeaders() {
	std::string	line;

	//Reads line by line until it finds an empty line
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

void	POST::extractFirstLine() {
	std::string	line;

	this->requestStream >> line;
	this->requestStream >> this->pathToRessource;
	this->requestStream >> this->HTTPversion;
	std::cout  << "path-to-resource: " << this->pathToRessource << RESET << std::endl;
	std::cout  << "HTTP: " << this->HTTPversion << RESET << std::endl;
	requestStream.clear();
	requestStream.seekg(0);
}

//We extract all the content of a POST request
POST::POST(Webserv _server, int serverFD, int clientFD, std::string &clientInput)
	: contentLength(0) {

	(void)_server;
	(void)serverFD;
	(void)clientFD;
	this->requestStream.str(clientInput);
	std::string line;
	std::cout << std::endl << "--------POST request---------" << std::endl;
	extractFirstLine();
	extractHeaders();
	extractBody(clientFD);
}

POST::~POST(void) {
}

