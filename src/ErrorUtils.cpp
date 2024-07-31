#include "Webserv.hpp"

void sendErrorResponse(int clientSocket, std::string statusCode,
                       const std::string &statusMessage)
{
  std::string response
      = "HTTP/1.1 " + statusCode + " " + statusMessage + "\r\n";
  response += "Content-Type: text/plain\r\n\r\n";
  response += statusMessage + "\n";
  if (sendall(clientSocket, response.c_str(), response.size()) == -1)
    perror("Data failed to be sent to the client");
}