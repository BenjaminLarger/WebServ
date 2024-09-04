/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   POST.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isporras <isporras@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: Invalid date        by                   #+#    #+#             */
/*   Updated: 2024/09/04 11:57:08 by isporras         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "POST.hpp"
#include "Webserv.hpp"
#include "core.hpp"

std::string POST::extractBody()
{
  std::string body;

  if (contentLength > 0)
  {
    char *buffer = new char[contentLength + 1];

    //Reads the content of the body from the actual position of the stream
    requestStream.read(buffer, contentLength);
    buffer[contentLength] = '\0';
    //Converts the buffer to a string to make it easier to manipulate
    body = buffer;
    delete[] buffer;
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
}

void POST::extractFirstLine()
{
  std::string line;

  this->requestStream >> line;
  this->requestStream >> this->pathToResource;
  this->requestStream >> this->HTTPversion;
  //Reset the stream to the beginning
  requestStream.clear();
  requestStream.seekg(0);
}

POST::POST(Webserv &webserv, ClientInfo &client, int clientFD,
           std::vector<char> &clientInput, const ServerConfig &serverConfig,
           std::string &_boundary)
    : clientInputVector(clientInput),
      clientInputString(clientInput.begin(), clientInput.end()),
      contentLength(0), ClientFD(clientFD), serverConfig(serverConfig), clientID(client.req.sessionId)
{
  std::string body;
  std::map<std::string, std::string> formValues;
  std::string path = client.req.pathOnServer;

  this->requestStream.str(clientInputString);
  extractFirstLine();
  extractHeaders();
  if (isFile(path))
  {
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
		if(client.req.URIpath == "/signup-form")
		{
			body = extractBody();
			// if (!name.empty() && !password.empty())
			// check if name already in db, if yes return error
			// save in db
			// return page sign up successful or not			
		}
		if(client.req.URIpath == "/login-form")
		{
			body = extractBody();
			// if (!name.empty() && !password.empty())
			// check if name already in db, if no return error
			// add sessionid of client to container of logged in users
			// return page sign up successful or not			
		}
		else 
		{
			body = extractBody();
			formValues = formValuestoMap(body);
			saveInLogFile(formValues);
			client.response = createPostOkResponse(formValues, client.req);
  	}
    }
	else if (!strncmp(contentType.c_str(), "plain/text", 9))
	{
		body = extractBody();
		client.response = createPostOkResponsePlainText(body, client.req);
	}
  else if (!strncmp(contentType.c_str(), "multipart/form-data", 19))
  {
    if (extractMultipartFormData(_boundary) == SUCCESS)
    {
      client.req.buffer.clear();
      saveInLogFile(_formValues);
      if (lineIsEmpty(contentMap[2].filename)
          == true)
        client.response = createPostOkResponse(_formValues, client.req);
      else
        client.response = createPostOkResponseWithFile(_formValues, client.req);
    }
  }
  else
    throw HttpException(415, "Unsupported Media Type.");

  if (!client.response.empty())
  {
    client.totalToSend = client.response.size();
    client.totalBytesSent = 0;
  }
}

POST::~POST(void) {}
