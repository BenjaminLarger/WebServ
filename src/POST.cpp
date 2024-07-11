#include "../inc/POST.hpp"
#include "../inc/Dependencies.hpp"

POST::POST(Webserv _server, int serverFD, int clientFD,
           std::string &clientInput)
    : requestStream(clientInput)
{

  (void)_server;
  (void)serverFD;
  (void)clientFD;

  std::string line;
  std::cout << clientInput << std::endl;
}
/* while (std::getline(this->requestStream, line) && line != "\r")
{
  if (line.find("Content-Length:") != std::string::npos)
  {
    std::istringstream isLine(line);
    std::string key;
    isLine >> key;
    isLine >> this->contentLength;
  }
} */
POST::~POST(void) {}