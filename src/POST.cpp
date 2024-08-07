/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   POST.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blarger <blarger@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/01 20:08:18 by demre             #+#    #+#             */
/*   Updated: 2024/08/07 17:26:45 by blarger          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "POST.hpp"
#include "Webserv.hpp"
#include "HttpExceptions.hpp"

//Send form data to a URL and get a response back

// void POST::sendResponse(int clientFD, std::string responseBody)
// {
//   //The format of an HTTP response is defined by the HTTP specification (RFC 2616 for HTTP/1.1).
//   //Here it is convenient to use ostring to concatenate
//   std::ostringstream response;
//   //Status Line: Specifies the HTTP version, status code, and status message.
//   response << "HTTP/1.1 200 OK\r\n";
//   //Headers: Metadata about the response.
//   response << "Content-Type: text/html\r\n";
//   response << "Content-Length: " << responseBody.size() << "\r\n";
//   response << "\r\n";
//   response << responseBody;

//   std::string responseStr = response.str();
//   // std::cout << "responseStr: \n" << responseStr << std::endl;
//   //send function is similar to write, but it is specific to socket.
//   //Supports additional flags to modify behavior (e.g., MSG_NOSIGNAL to prevent sending a SIGPIPE signal).
//   //Syntax: ssize_t send(int sockfd, const void *buf, size_t len, int flags);

//   if (sendall(clientFD, responseStr.c_str(), responseStr.size()) == -1)
//     throw HttpException(
//       "500", "Internal Server Error: Data failed to be sent to the client");
// }

std::string POST::buildPostHtmlResponse()
{
  std::string responseBody;
  std::map<std::string, std::string> formValues;
  std::istringstream bodyStream(body);
  std::string keyValuePair;

  while (std::getline(bodyStream, keyValuePair, '&'))
  {
      size_t pos = keyValuePair.find('=');
      if (pos != std::string::npos)
      {
          std::string key = keyValuePair.substr(0, pos);
          std::string value = keyValuePair.substr(pos + 1);
          formValues[key] = value;
      }
  }
  responseBody = "<html><body><h1>Form data received</h1><table>";
  for (std::map<std::string, std::string>::iterator it = formValues.begin(); it != formValues.end(); ++it)
  {
      responseBody += "<tr><td>" + it->first + "</td><td>" + it->second + "</td></tr>";
  }
  return responseBody;
}

void POST::extractBody(int clientFD)
{
	//std::cout << "length = " << contentLength << std::endl;
  // Read body
  try
  {
    if (contentLength > 0)
    {
      char *buffer = new char[contentLength + 1];

      //Reads the content of the body from the actual position of the stream
      requestStream.read(buffer, contentLength);
      buffer[contentLength] = '\0';
      //Converts the buffer to a string to make it easier to manipulate
      body = buffer;
      delete[] buffer;
      std::cout << "body: " << body << std::endl;
      // 200 = code for success received, OK = brief description of the status, text/plain = type of the response
      sendRGeneric(ClientFD, ResponseHtmlOkBody(buildPostHtmlResponse()));
    }
    else
      sendErrorResponse(clientFD, "411", "Length required", "");
  }
  catch (const HttpException &e)
  {
    std::cerr << RED << "Error: " << e.what() << RESET << '\n';
    // sendDefaultErrorPage(clientFD, e.getStatusCode(), e.getErrorMessage(),
    //                      serverConfigs[serverIndex].errorPages);
  }
  //close(clientFD);
}

std::string POST::makeCopy(const std::string &original)
{
    if (original.length() < 4) {
        return "";
    }
    std::string copy;
    for (size_t i = 4; i < original.length(); ++i) {
        copy += original[i];
    }
		std::cout << RED << copy << RESET << std::endl;
    return copy;
}

void POST::extractHeaders()
{
  std::string line;
	bool				isFirstLine = true;

  //Reads line by line until it finds an empty line
  while (std::getline(requestStream, line) && line != "\r")
  {
	std::cout << MAGENTA << line << std::endl;
    size_t colonPos = line.find(":");
    if (colonPos != std::string::npos)
    {
      std::string headerName = line.substr(0, colonPos);
      std::string headerValue = line.substr(colonPos + 1);
      // Trim whitespace
      headerValue.erase(0, headerValue.find_first_not_of(" \t"));
      headerValue.erase(headerValue.find_last_not_of(" \t") + 1);
      this->headers[headerName] = headerValue;
			isFirstLine = false;
      if (headerName == "Content-Length")
        contentLength = std::atoi(headerValue.c_str());
      else if (headerName == "Content-Type")
        contentType = headerValue;
      else if (headerName == "Host")
        host = headerValue;
    }
		else if (isFirstLine == false)
			break;
  }

	std::cout << RESET << "\nEXTRACT HEADER :\n" ;
  std::cout << YELLOW << "content-type: " << this->contentType << std::endl;
  std::cout << "content-length: " << this->contentLength << std::endl;
  std::cout << "host: " << this->host << RESET << std::endl;
}

void POST::extractFirstLine()
{
  std::string line;

  this->requestStream >> line;
  this->requestStream >> this->pathToRessource;
  this->requestStream >> this->HTTPversion;
  std::cout << "\nFIRST LINE :\n" ;
  std::cout << YELLOW << "path-to-resource: " << this->pathToRessource << RESET
            << std::endl;
  std::cout << YELLOW << "HTTP: " << this->HTTPversion << RESET << std::endl;
  //Reset the stream to the beginning
  requestStream.clear();
  requestStream.seekg(0);
}

//We extract all the content of a POST request
POST::POST(int serverFD, int clientFD, std::string &clientInput)
    : contentLength(0), ClientFD(clientFD)
{
  (void)serverFD;
  (void)clientFD;
  this->requestStream.str(clientInput);
  std::string line;
  std::cout << std::endl << "--------POST request---------" << std::endl;
  extractFirstLine();
  extractHeaders();
  if (!strncmp(contentType.c_str(), "application/x-www-form-urlencoded", 33))
  	extractBody(clientFD);
  else if (!strncmp(contentType.c_str(), "multipart/form-data", 19))
	{

		if (extractMultipartFormData() == SUCCESS)
			clientInput.erase();
	}
}

POST::~POST(void) {}
