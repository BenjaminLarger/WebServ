/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   POST.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blarger <blarger@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: Invalid date        by                   #+#    #+#             */
/*   Updated: 2024/09/01 19:03:06 by blarger          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "POST.hpp"
#include "Webserv.hpp"
#include "core.hpp"

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
    //std::cout << MAGENTA << line << std::endl;
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

//We extract all the content of a POST request
POST::POST(Webserv &webserv, ClientInfo &client, int clientFD,
           std::vector<char> &clientInput, const ServerConfig &serverConfig,
           std::string &_boundary)
    : clientInputVector(clientInput),
      clientInputString(clientInput.begin(), clientInput.end()),
      contentLength(0), ClientFD(clientFD), serverConfig(serverConfig)
{
  //std::string response;
  std::string body;
  std::map<std::string, std::string> formValues;
  std::string path = client.req.pathOnServer;

  this->requestStream.str(clientInputString);
  std::cout << std::endl << "--------POST request---------" << std::endl;
  extractFirstLine();
  extractHeaders();
  // Depending on the content type of the form the body is formatted in a different way
  std::cout << ORANGE << "buffer = " <<client.req.buffer << RESET << std::endl;
	std::cout << ORANGE << "ct = " <<contentType << RESET << std::endl;
  if (isFile(path))
  {
    std::cout << YELLOW << "This is a file\n" << RESET << std::endl;
    std::string fileName, extension;
    getFileNameAndExtension(path, fileName, extension);
    if (extension == "php" || extension == "py")
    {
			if (!strncmp(contentType.c_str(), "multipart/form-data", 19))
			 extractMultipartFormData(_boundary);
			webserv.executeScript(path, extension, client);
			
      return; // don't remove. Shouldn't set response after script execution.
    }
  }
  else if (!strncmp(contentType.c_str(), "application/x-www-form-urlencoded",
                    33))
  {
    body = extractBody();
    formValues = formValuestoMap(body);
    saveInLogFile(formValues);
    client.response = createPostOkResponse(formValues);
    //client.totalToSend = client.response.size();
    client.req.buffer.clear(); //may have to delete
  }
	else if (!strncmp(contentType.c_str(), "plain/text", 9))
	{
		body = extractBody();
		client.response = createPostOkResponsePlainText(body);
	}
  else if (!strncmp(contentType.c_str(), "multipart/form-data", 19))
  {
    if (extractMultipartFormData(_boundary) == SUCCESS)
    {
      std::cout << "RETURN SUCCESS\n";
      client.req.buffer.clear();
      std::cout << "multipart/form-data return SUCCESS\n";
      saveInLogFile(_formValues);
      if (lineIsEmpty(contentMap[2].filename)
          == true) //no file has been uploaded
        client.response = createPostOkResponse(_formValues);
      else
        client.response = createPostOkResponseWithFile(_formValues);
      //client.totalToSend = client.response.size();
    }
  }
  else
  {
    std::cout << RED << "POST method unfound\n" << RESET;
    std::cout << RED << "Content-Type: " << contentType << RESET << std::endl;
    throw HttpException(415, "Unsupported Media Type.");
  }

  if (!client.response.empty())
  {
    client.totalToSend = client.response.size();
    client.totalBytesSent = 0;
  }
}

POST::~POST(void) {}
