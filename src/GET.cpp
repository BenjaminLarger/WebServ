/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   GET.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: demre <demre@student.42malaga.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/09 11:49:01 by blarger           #+#    #+#             */
/*   Updated: 2024/08/08 16:21:01 by demre            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "GET.hpp"
#include "CGI.hpp"
#include "core.hpp"

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

      response = composeOkHtmlResponse(
          generateDirectoryListing(pathToResource + "/", contents));
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