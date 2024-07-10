/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   GET.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: demre <demre@student.42malaga.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/09 11:49:01 by blarger           #+#    #+#             */
/*   Updated: 2024/07/10 15:23:24 by demre            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "GET.hpp"

//-----> use a map container to associate a string key found with a function that set  object value (eg: key = "Host:" || function = setHost(std::string value) {this->host = value})
/* bool findHeader(const std::map<std::string,function> &headers,
                const std::string &targetHeader)
{
  for (size_t i = 0; i < headers.size(); ++i)
  { // Loop through all headers
    if (headers[i] == targetHeader)
    {              // Check if the current header matches the target
      return true; // Header found
    }
  }
  return false; // Header not found
} */

void GET::findHeader(std::string &key, std::istringstream &isLine)
{
  std::string newKey;
  if (key == "Host:")
  {
    isLine >> this->host;
    isLine >> newKey;
    if (key != newKey)
      findHeader(newKey, isLine);
  }
  else if (key == "User-Agent:")
  {
    isLine >> this->userAgent;
    std::cout << YELLOW << "userAgent = " << this->host << RESET << std::endl;
    isLine >> newKey;
    std::cout << YELLOW << "key = " << key << RESET << std::endl;
    if (key != newKey)
      findHeader(newKey, isLine);
  }
  else if (key == "Accept:")
  {
    isLine >> this->accept;
    std::cout << YELLOW << "accept = " << this->accept << RESET << std::endl;
    isLine >> newKey;
    std::cout << YELLOW << "key = " << key << RESET << std::endl;
    if (key != newKey)
      findHeader(newKey, isLine);
  }
}

int countJumpLine(std::string str)
{
  int count = 0;

  for (int i = 0; str[i]; i++)
  {
    if (str[i] == '\n')
      count++;
  }
  return (count);
}

std::string GET::extractHtmlContent(const std::string &filePath)
{
  std::ifstream file(filePath.c_str());
  if (!file.is_open())
    throw std::runtime_error("Could not open file: " + filePath);

  std::stringstream buffer;
  buffer << file.rdbuf();

  return buffer.str();
}

void GET::sendResponse(int clientFD)
{
  std::string responseBody;
  //The format of an HTTP response is defined by the HTTP specification (RFC 2616 for HTTP/1.1).
  //Body: The actual content (e.g., HTML, JSON).
  if (this->pathToRessource == "/silence")
    responseBody = extractHtmlContent("public/body/silence.html");
  else if (this->pathToRessource == "/")
    responseBody = extractHtmlContent("public/body/main.html");
  else
    responseBody = extractHtmlContent("public/error_pages/404.html");
  //Here it is convenient to use ostring to concatenate
  std::ostringstream response;
  //Status Line: Specifies the HTTP version, status code, and status message.
  response << "HTTP/1.1 200 OK\r\n";
  //Headers: Metadata about the response.
  response << "Content-Type : " << responseBody.size() << "\r\n";
  response << "Connection close\r\n";
  response << "\r\n";
  response << responseBody;

  std::string responseStr = response.str();
  //send function is similar to write, but it is specific to socket.
  //Supports additional flags to modify behavior (e.g., MSG_NOSIGNAL to prevent sending a SIGPIPE signal).
  //Syntax: ssize_t send(int sockfd, const void *buf, size_t len, int flags);
  if (send(clientFD, responseStr.c_str(), responseStr.size(), 0) == -1)
    throw(std::runtime_error("fail sending the message"));
}

GET::GET(Webserv server, int serverFD, int clientFD, std::string &clientInput)
{
  (void)server;
  (void)clientInput;
  (void)serverFD;
  (void)clientFD;

  std::cout << RED << countJumpLine(clientInput)
            << " jumplines in client request!" << RESET << std::endl;
  if (countJumpLine(clientInput) <= 3)
    return;
  std::istringstream isLine(clientInput);
  std::string key;

  std::cout << MAGENTA << clientInput << RESET << std::endl;
  isLine >> key;
  isLine >> this->pathToRessource;
  isLine >> this->HTTPversion;
  // END OF FIRST LINE

  // START PARSING HEADER AND BODY
  isLine >> key; // Header

  // Read client input => Using flag ? using a "readline"
  if (key == "Host:" || key == "User-Agent:" || key == "Accept:")
    findHeader(key, isLine);
  //write(clientFD, "GET client info\n", 14);
  std::cout << "pathToRessource = " << YELLOW << this->pathToRessource << RESET
            << std::endl;
  std::cout << "HTTPversion = " << YELLOW << this->HTTPversion << RESET
            << std::endl;
  std::cout << "host = " << YELLOW << this->host << RESET << std::endl;
  std::cout << "userAgent = " << YELLOW << this->userAgent << RESET
            << std::endl;
  std::cout << "accept = " << YELLOW << this->accept << RESET << std::endl;
  std::cout << RESET;
  clientInput.erase();
  try
  {
    sendResponse(clientFD);
  }
  catch (const std::exception &e)
  {
    std::cerr << RED << e.what() << RESET << '\n';
  }
}

GET::GET(){};

GET::~GET(){};