/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   GET.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isporras <isporras@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/09 11:49:01 by blarger           #+#    #+#             */
/*   Updated: 2024/08/08 17:21:58 by isporras         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "GET.hpp"
#include "CGI.hpp"
#include "core.hpp"

void GET::setAccept(const std::string &_accept) { this->accept = _accept; }
void GET::setHost(const std::string &_host) { this->host = _host; }
void GET::setUserAgent(const std::string &_userAgent) { this->userAgent = _userAgent; }

std::string createHtmlDeleteRequest(std::vector<std::string> files, std::string uploadspth)
{
    std::ostringstream html;
    
    html << extractHtmlContentFromFile("./var/www/delete/delete_request.html");
    html << "<body>";
    html << "<h1>Files in " << uploadspth << "</h1>";
    html << "<table border='1'>";
    html << "<tr><th>File Name</th><th>Action</th></tr>";

    for (std::vector<std::string>::const_iterator it = files.begin(); it != files.end(); ++it)
    {
        html << "<tr>";
        html << "<td>" << *it << "</td>";
        html << "<td>";
        html << "<button onclick=\"deleteFile('" << *it << "', this)\">Delete</button>";
        html << "</td>";
        html << "</tr>";
    }

    html << "</table>";
    html << "</body>";
    html << "</html>";

    return html.str();
}

std::string manageDeleteEndPoint()
{
  std::string uploadspth = "var/www/uploads";
  std::vector<std::string> files = listFilesInDirectory(uploadspth);
  std::string htmlDeleteBody;
  
  htmlDeleteBody = createHtmlDeleteRequest(files, uploadspth);
  return (htmlDeleteBody);
}

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

std::string GET::handleLocations(std::string pathToResource)
{
  std::string response;
  std::map<std::string, LocationConfig> locations = serverConfig.locations;
  std::map<std::string, LocationConfig>::const_iterator it;

  pathOrParentFolderExistsInLocations(pathToResource, locations, it);

  if (it != locations.end()) // it->first
  {
    std::cout << "pathToResource: " << pathToResource
              << ", found location: " << it->first << std::endl;
    std::string path;
    std::string root = it->second.root;

    if (it->first == "/delete")
      return (composeOkHtmlResponse(manageDeleteEndPoint()));
    // is already a folder
    if (!pathToResource.empty()
        && pathToResource[pathToResource.size() - 1] == '/')
      path = "." + root + pathToResource;
    // is a file inside a folder located at it->first
    else if (pathToResource != it->first)
      path = "." + root + pathToResource;
    // is a folder without a "/"
    else
      path = "." + root + pathToResource + "/";

    std::cout << "path: '" << path
              << "' isDirectory(path): " << isDirectory(path) << std::endl;

    // Check if the location has a redirection
    if (it->second.redirection.first)
    {
      std::cout << "redirection: " << it->second.redirection.first << " "
                << it->second.redirection.second << std::endl;

      response = createRedirectResponse(it->second.redirection.first,
                                        it->second.redirection.second);
      return (response);
    }
    // Check if the location has a file to serve
    else if (!it->second.index.empty())
    {
      path += it->second.index;
      response = composeOkHtmlResponse(extractHtmlContentFromFile(path));
      return (response);
    }
    // If location doesn't have a file, is a folder, and autoindex on
    else if (it->second.index.empty() && isDirectory(path)
             && it->second.autoIndexOn)
    {
      std::vector<std::string> contents = listDirectoryContent(path);

      response = composeOkHtmlResponse(createFileListHtml(path));
      return (response);
    }
    // pathToResource doesn't match a location, but is contained in one which is a folder
    else if (pathToResource != it->first && !isDirectory(path)
             && isDirectory("." + root + it->first))
    {
      // std::cout << "pathToResource != it->first && !isDirectory(path). "
      //              "Checking isDirectory(\".\" + root + it->first): "
      //           << isDirectory("." + root + it->first) << std::endl;

      // Save binary file to char vector
      std::vector<char> fileContent = readFile(path);
      if (fileContent.empty())
        throw HttpException(404, "Not Found.");

      response = composeFileResponse(fileContent, pathToResource);
      return (response);
    }
    // Else: no file, is a folder, but autoindex off
    else
    {
      throw HttpException(
          403, "You don't have permission to access this directory.");
    }
  }
  else
  {
    throw HttpException(404, "Not Found");
  }
}

GET::GET(ClientInfo &client, int clientFD, std::string &clientInput,
         const ServerConfig &serverConfig)
    : serverConfig(serverConfig)
{
  if (countJumpLine(clientInput)
      < 3) //Change with if receive a blank line => erase buffer
    return;
  std::istringstream iss(clientInput);
  std::string key;

  clientInput.erase();

  try
  {
    // getresponse()
    // Body: The actual content (e.g., HTML, JSON).
    std::string response;
    if (client.req.pathToRessource.find(".php") != std::string::npos)
      response = composeOkHtmlResponse(executePhp(client.req.pathToRessource));
    else
      response = handleLocations(client.req.pathToRessource);
    sendRGeneric(clientFD, response);
    // std::cout << "response sent." << std::endl;
  }
  catch (const HttpException &e)
  {
    sendDefaultErrorPage(clientFD, e.getStatusCode(),
                         getReasonPhrase(e.getStatusCode()),
                         serverConfig.errorPages);
  }
}

GET::~GET() {};