/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   POST.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isporras <isporras@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: Invalid date        by                   #+#    #+#             */
/*   Updated: 2024/08/09 19:33:44 by isporras         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */



#include "POST.hpp"
#include "Webserv.hpp"
#include "core.hpp"


std::string POST::makeCopy(const std::string &original)
{
  if (original.length() < 4)
  {
    return "";
  }
  std::string copy;
  for (size_t i = 4; i < original.length(); ++i)
  {
    copy += original[i];
  }
  std::cout << RED << copy << RESET << std::endl;
  return copy;
}

std::string POST::extractBody()
{
    std::string body;

    // Read body
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
    }
    else
      throw HttpException(400, "Bad Request: Content-Length is missing");

    return (body);
}

void POST::extractHeaders()
{
  std::string line;
  bool isFirstLine = true;
  std::map<std::string, std::string> headers;

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
      headers[headerName] = headerValue;
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

  std::cout << RESET << "\nEXTRACT HEADER :\n";
  std::cout << YELLOW << "content-type: " << this->contentType << std::endl;
  std::cout << "content-length: " << this->contentLength << std::endl;
  std::cout << "host: " << this->host << RESET << std::endl;
}

void POST::extractFirstLine()
{
  std::string line;

  this->requestStream >> line;
  this->requestStream >> this->pathToResource;
  this->requestStream >> this->HTTPversion;
  std::cout << "\nFIRST LINE :\n";
  std::cout << YELLOW << "path-to-resource: " << this->pathToResource << RESET
            << std::endl;
  std::cout << YELLOW << "HTTP: " << this->HTTPversion << RESET << std::endl;
  //Reset the stream to the beginning
  requestStream.clear();
  requestStream.seekg(0);
}

std::string	createPostUploadOkResponse()
{
	std::ostringstream response;
  response << "HTTP/1.1 204 No Content\r\n";
  response << "Content-Length: 0\r\n";
  response << "Connection: close\r\n\r\n";

  return (response.str());
}

//We extract all the content of a POST request
POST::POST(ClientInfo &client, int serverFD, int clientFD,
           std::string &clientInput, const ServerConfig &serverConfig)
    : contentLength(0), ClientFD(clientFD), serverConfig(serverConfig)
{
  std::string response;
  std::string body;
  std::map<std::string, std::string> formValues;
  (void)client;
  (void)serverFD;
  (void)clientFD;

  this->requestStream.str(clientInput);
  std::cout << std::endl << "--------POST request---------" << std::endl;
  extractFirstLine();
  extractHeaders();
  // Depending on the content type of the form the body is formatted in a different way
  if (!strncmp(contentType.c_str(), "application/x-www-form-urlencoded", 33))
  {
    body = extractBody();
    formValues = formValuestoMap(body);
    saveInLogFile(formValues);
    response = createPostOkResponse(formValues);
  }
  else if (!strncmp(contentType.c_str(), "multipart/form-data", 19))
  {

    if (extractMultipartFormData(clientInput) == SUCCESS)
		{
      clientInput.erase();
			/* std::string response = createPostUploadOkResponse();
			std::cout << GREEN << "Sending post OK response" << RESET << std::endl;
			sendRGeneric(clientFD, response); */
		}
  }
  else
    throw HttpException(415, "Unsupported Media Type.");
  //I think we should send the generic response from here and use just the divided part to build the response depending on the type
  sendRGeneric(clientFD, response);
}

POST::~POST(void) {}
