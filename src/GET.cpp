/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   GET.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: demre <demre@student.42malaga.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/09 11:49:01 by blarger           #+#    #+#             */
/*   Updated: 2024/08/11 17:52:51 by demre            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "GET.hpp"
#include "CGI.hpp"
#include "core.hpp"

std::string GET::getResponseAtLocation(ClientRequest &req)
{
  std::string URI = req.URI;
  std::string response;
  std::map<std::string, LocationConfig> locations = serverConfig.locations;
  std::map<std::string, LocationConfig>::const_iterator it;

  //delete this and replace it by req...
  findURIstartInLocations(req.URI, locations, it);

  if (it != locations.end())
  {
    std::cout << RED << "req.pathFolder: " << req.pathFolder << RESET
              << std::endl;

    std::string path = req.pathOnServer;

    if (it->first == "/delete")
      return (composeOkHtmlResponse(manageDeleteEndPoint()));
    // Check if the location has a redirection
    else if (it->second.redirection.first)
    {
      std::cout << "redirection: " << it->second.redirection.first << " "
                << it->second.redirection.second << std::endl;

      response = createRedirectResponse(it->second.redirection.first,
                                        it->second.redirection.second);
      return (response);
    }
    // if doesn't exist as a file or folder on the server
    else if (!isDirectory(path) && !isFile(path))
    {
      throw HttpException(404, "pathOnServer not a file or folder on server");
    }
    else if (isFile(path))
    {
      std::string fileName, extension;
      getFileNameAndExtension(path, fileName, extension);

      std::cout << RED << "fileName: " << fileName
                << ", extension: " << extension << RESET << std::endl;

      // if (!hasQuestionMarkOrEqual(path) && !hasPunctuation(extension))
      // if file is html
      if (extension == "html")
      {
        std::cout << RED << "file is html" << RESET << std::endl;
        response = composeOkHtmlResponse(extractHtmlContentFromFile(path));
      }
      // if file is php
      else if (extension == "php" || extension == "py")
      {
        std::cout << RED << "file is a script in " << extension << RESET
                  << std::endl;
        response = composeOkHtmlResponse(executeScript(path, extension));
      }
      // other files
      else if (extension.size())
      {
        std::cout << RED << "file has other type: " << extension << RESET
                  << std::endl;
        std::vector<char> fileContent = readFile(path);
        if (fileContent.empty())
          throw HttpException(404, "File to read not found.");

        response = composeFileResponse(fileContent, URI);
      }
      // has query string /...?...=...
      else
      {
      }
      return (response);
    }
    // Check if the location has a file to serve
    else if (!it->second.index.empty())
    {
      std::cout << RED << "it->second.serverPath: " << it->second.serverPath
                << RESET << std::endl;

      path += "/" + it->second.index;
      std::cout << RED << "!it->second.index.empty() path: " << path << RESET
                << std::endl;

      response = composeOkHtmlResponse(extractHtmlContentFromFile(path));
      return (response);
    }
    // If location doesn't have a file, is a folder, and autoindex on
    else if (it->second.index.empty() && isDirectory(path)
             && it->second.autoIndexOn)
    {
      std::cout << RED << "listDirectoryContent: " << path << RESET
                << std::endl;
      std::vector<std::string> contents = listDirectoryContent(path);

      // response = composeOkHtmlResponse(createFileListHtml(path));
      response
          = composeOkHtmlResponse(generateDirectoryListing(path, contents));
      return (response);
    }
    // URI doesn't match a location, but is contained in one which is a folder
    // else if (URI != it->first && !isDirectory(path)
    //          && isDirectory(req.pathFolderOnServer))
    // {
    //   std::cout << RED << "readFile path: " << path << RESET << std::endl;

    //   // Save binary file to char vector
    //   std::vector<char> fileContent = readFile(path);
    //   if (fileContent.empty())
    //     throw HttpException(404, "Not Found.");

    //   response = composeFileResponse(fileContent, URI);
    //   return (response);
    // }
    // Else: no file, is a folder, but autoindex off
    else
    {
      throw HttpException(
          403, "You don't have permission to access this directory." + path);
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
    // Body: The actual content (e.g., HTML, JSON).
    std::string response = getResponseAtLocation(client.req);
    sendRGeneric(clientFD, response);
    // std::cout << "response sent." << std::endl;
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

GET::~GET() {};