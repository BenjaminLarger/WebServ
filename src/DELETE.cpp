/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   DELETE.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blarger <blarger@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/07 15:57:35 by isporras          #+#    #+#             */
/*   Updated: 2024/08/08 17:34:21 by blarger          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "DELETE.hpp"
#include "HttpExceptions.hpp"
#include "core.hpp"

void DELETE::setHost(const std::string &_host) { this->host = _host; }
void DELETE::setAuthorization(const std::string &_authorization)
{
  this->authorization = _authorization;
}
void DELETE::setIfMatch(const std::string &_if_match)
{
  this->if_match = _if_match;
}
std::string DELETE::getHost(void) const { return this->host; }
std::string DELETE::getAuthorization(void) const { return this->authorization; }
std::string DELETE::getIfMatch(void) const { return this->if_match; }

void DELETE::checkPreconditions(std::string expectedEtag,
                                std::string expectedAuth)
{
  // std::cout << "Expected Etag: " << expectedEtag << std::endl;
  // std::cout << "If-Match: " << getIfMatch() << std::endl;
  // std::cout << "Expected Auth: " << expectedAuth << std::endl;
  // std::cout << "Authorization: " << getAuthorization() << std::endl;
  if (expectedEtag != getIfMatch())
    throw HttpException(412, "Precondition Failed: ETag does not match");
  if (expectedAuth != getAuthorization())
    throw HttpException(401, "Unauthorized");
}

void DELETE::findHeader(std::istringstream &isLine)
{
  std::string line;
  bool isFirstLine = true;
  std::map<std::string, std::string> headers;
  isLine.clear();
  isLine.seekg(0);
  //Reads line by line until it finds an empty line
  while (std::getline(isLine, line) && line != "\r")
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
      if (headerName == "Host")
        setHost(headerValue);
      else if (headerName == "Authorization")
        setAuthorization(headerValue);
      else if (headerName == "If-Match")
        setIfMatch(headerValue);
    }
    else if (isFirstLine == false)
      break;
  }

  std::cout << RESET << "\nEXTRACT HEADER :\n";
  std::cout << YELLOW << "Host: " << getHost() << std::endl;
  std::cout << "Authorization: " << getAuthorization() << std::endl;
  std::cout << "If-Match: " << getIfMatch() << RESET << std::endl;
}

void DELETE::parseRequest(std::string &clientInput)
{
  std::cout << MAGENTA << clientInput << RESET << std::endl;
  std::istringstream isLine(clientInput);
  std::string key;

  isLine >> key;
  isLine >> pathToRessource;
  isLine >> HTTPversion;
  // START PARSING HEADER
  isLine >> key;

  findHeader(isLine);
}

DELETE::DELETE(ClientInfo &client, int clientFD, std::string &clientInput,
               const ServerConfig &_serverConfig)
    : serverConfig(_serverConfig)
{
  (void)client;
  std::string response;

  try
  {
    parseRequest(clientInput);
    // Example of tag. Then we maybe need to create them dynamicaly
    std::string expectedEtag = "anytag";
    std::string expectedAuth = "anytoken";
    //checkPreconditions(expectedEtag, expectedAuth);

    pathToRessource.insert(pathToRessource.begin(), '.');
    std::cout << "Path to ressource: " << pathToRessource << std::endl;
    if (remove(pathToRessource.c_str()) != 0)
      throw HttpException(500, "Internal Server Error");
    else
      response = composeOkHtmlResponse(
          extractHtmlContentFromFile("var/www/delete/delete_response.html"));
    std::cout << GREEN << "Sending delete OK response" << RESET << std::endl;
    sendRGeneric(clientFD, response);
  }
  catch (const HttpException &e)
  {
    std::cerr << RED << "Error: " << e.getStatusCode() << " " << e.what()
              << RESET << '\n';

    sendDefaultErrorPage(clientFD, e.getStatusCode(),
                         getReasonPhrase(e.getStatusCode()),
                         serverConfig.errorPages);
  }
}

DELETE::~DELETE() {}