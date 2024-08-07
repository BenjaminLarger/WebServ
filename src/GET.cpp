/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   GET.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: demre <demre@student.42malaga.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/09 11:49:01 by blarger           #+#    #+#             */
/*   Updated: 2024/08/07 15:08:32 by demre            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "GET.hpp"
#include "CGI.hpp"
#include "core.hpp"

void GET::setAccept(const std::string &_accept) { this->accept = _accept; }
void GET::setHost(const std::string &_host) { this->host = _host; }
void GET::setUserAgent(const std::string &_userAgent)
{
  this->userAgent = _userAgent;
}
//-----> use a map container to associate a string key found with a function that set  object value (eg: key = "Host:" || function = setHost(std::string value) {this->host = value})
void GET::findHeader(std::string &key, std::istringstream &isLine)
{
  std::string newKey;
  typedef void (GET::*HeaderSetter)(const std::string &);
  std::map<std::string, HeaderSetter> headersMap;

  headersMap["Host:"] = &GET::setHost;
  headersMap["User-Agent:"] = &GET::setUserAgent;
  headersMap["Accept:"] = &GET::setAccept;

  std::map<std::string, HeaderSetter>::iterator it = headersMap.begin();

  while (it != headersMap.end())
  { // Loop through all headers
    if (it->first == key)
    {
      std::cout << GREEN << "Header found ! " << key << std::endl;
      isLine >> newKey;
      std::cout << ORANGE << "new key = " << newKey << std::endl;
      while (isLine.peek() != '\n' && isLine.peek() != 13
             && isLine.peek() != EOF)
      {
        std::string temp;
        isLine >> temp;
        newKey += temp;
      }
      std::cout << RED << "new key = " << newKey << std::endl;
      if (key != newKey)
      {
        (this->*(it->second))(newKey);
        isLine >> newKey;
        findHeader(newKey, isLine);
      }
    }
    ++it;
  }
  isLine >> newKey;
  if (key != newKey)
    findHeader(newKey, isLine);
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
  std::cout << "filePath: " << filePath << std::endl;
  std::ifstream file(filePath.c_str());
  if (!file.is_open())
    throw std::runtime_error("Could not open file: " + filePath);

  std::stringstream buffer;
  buffer << file.rdbuf();
  buffer << "\r\n";

  return (buffer.str());
}

std::string GET::handleLocations(std::string pathToResource)
{
  std::string responseBody;
  std::map<std::string, LocationConfig> locations = serverConfig.locations;
  std::map<std::string, LocationConfig>::iterator it
      = locations.find(pathToResource);

  if (it != locations.end()) // pathToResource == it->first
  {
    std::string path;
    std::string root = it->second.root;

    if (!pathToResource.empty()
        && pathToResource[pathToResource.size() - 1] == '/')
      path = "." + root + pathToResource;
    else
      path = "." + root + pathToResource + "/";

    std::cout << "path: '" << path
              << "' isDirectory(path): " << isDirectory(path) << std::endl;

    if (pathToResource == "/favicon.ico")
    {
      return ("");
    }
    // Check if the location has a redirection
    if (it->second.redirection.first)
    {
      std::cout << "redirection: " << it->second.redirection.first << " "
                << it->second.redirection.second << std::endl;

      // HTTP/1.1 301 Moved Permanently
      // Location: http://www.example.com/new-url
      // Content-Length: 0
      responseBody = redirectionHeader(it->second.redirection.second);
      return (responseBody);
    }
    // Check if the location has a file to serve
    else if (!it->second.index.empty())
    {
      path += it->second.index;
      responseBody = ResponseHtmlOkBody(extractHtmlContent(path));
      return (responseBody);
    }
    // If location doesn't have a file, is a folder, and autoindex on
    else if (!it->second.index.size() && isDirectory(path)
             && it->second.autoIndexOn)
    {
      std::vector<std::string> contents = listDirectoryContent(path);

      responseBody = ResponseHtmlOkBody(
          generateDirectoryListing(pathToResource + "/", contents));
      return (responseBody);
    }
    // Else: no file, is a folder, but autoindex off
    else
    {
      throw HttpException(
          "403", "You don't have permission to access this directory.");
    }
  }
  else
  {
    throw HttpException("404", "Not Found");
  }
}

GET::GET(int clientFD, std::string &clientInput,
         const ServerConfig &serverConfig)
    : serverConfig(serverConfig)
{
  // (void)server;
  (void)clientInput;
  (void)clientFD;

  // std::cout << RED << countJumpLine(clientInput)
  //         << " jumplines in client request!" << RESET << std::endl;
  if (countJumpLine(clientInput)
      < 3) //Change with if receive a blank line => erase buffer
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
  /* if (key == "Host:" || key == "User-Agent:" || key == "Accept:")
     */
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
    // getResponseBody()
    // Body: The actual content (e.g., HTML, JSON).
    std::string responseBody;
    if (this->pathToRessource.find(".php") != std::string::npos)
      responseBody = ResponseHtmlOkBody(executePhp(this->pathToRessource));
    else
      responseBody = handleLocations(pathToRessource);
    sendRGeneric(clientFD, responseBody);
    // std::cout << "response sent." << std::endl;
  }
  catch (const HttpException &e)
  {
    std::cerr << RED << "Error: " << e.what() << RESET << '\n';
    sendDefaultErrorPage(clientFD, e.getStatusCode(), e.getErrorMessage(),
                         serverConfig.errorPages);
  }
}

GET::~GET() {};