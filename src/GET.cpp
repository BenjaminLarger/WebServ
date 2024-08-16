/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   GET.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: demre <demre@student.42malaga.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/09 11:49:01 by blarger           #+#    #+#             */
/*   Updated: 2024/08/16 17:55:06 by demre            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "GET.hpp"
#include "core.hpp"

std::string GET::getResponseAtLocation(Webserv &webserv, ClientRequest &req,
                                       int &clientFD)
{
  std::string URI = req.URIpath;
  std::string response;
  std::map<std::string, LocationConfig> locations = serverConfig.locations;
  std::map<std::string, LocationConfig>::const_iterator it;

  findURIstartInLocations(req.URIpath, locations, it);

  if (it != locations.end())
  {
    // std::cout << RED << "req.pathFolder: " << req.pathFolder << RESET
    //           << std::endl;

    std::string path = req.pathOnServer;

    if (it->first == "/delete")
      return (composeOkHtmlResponse(manageDeleteEndPoint(), req.buffer));
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
        response = composeOkHtmlResponse(extractHtmlContentFromFile(path),
                                         req.buffer);
      }
      // if file is a script in php or python
      else if (extension == "php" || extension == "py")
      {
        std::cout << RED << "file is a script in " << extension << RESET
                  << std::endl;

        webserv.executeScript(path, extension, req.queryString, clientFD);
        response = "";
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
      path += "/" + it->second.index;
      std::cout << RED << "file at: " << path << RESET << std::endl;

      response
          = composeOkHtmlResponse(extractHtmlContentFromFile(path), req.buffer);
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
      response = composeOkHtmlResponse(generateDirectoryListing(path, contents),
                                       req.buffer);
      return (response);
    }
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

GET::GET(Webserv &webserv, ClientInfo &client, const ServerConfig &serverConfig)
    : serverConfig(serverConfig)
{
  try
  {
    // if the get is a CGI script, we are adding the pipe to pollfd and clients vectors, so we need to get the index again to have the correct client.
    int clientFD = client.socketFD;
    std::string response
        = getResponseAtLocation(webserv, client.req, client.socketFD);

    size_t i = webserv.findClientIndexFromFD(clientFD);
    webserv.clients[i].response = response;
    webserv.clients[i].totalToSend = (!webserv.clients[i].response.empty()
                                          ? webserv.clients[i].response.size()
                                          : 0);
    webserv.clients[i].totalBytesSent = 0;
  }
  catch (const HttpException &e)
  {
    std::cerr << RED << "Error: " << e.getStatusCode() << " " << e.what()
              << RESET << '\n';

    size_t i = webserv.findClientIndexFromFD(client.socketFD);
    webserv.clients[i].response = composeErrorHtmlPage(
        e.getStatusCode(), getReasonPhrase(e.getStatusCode()),
        serverConfig.errorPages);
    webserv.clients[i].totalToSend = webserv.clients[i].response.size();
    webserv.clients[i].totalBytesSent = 0;
  }
}

GET::~GET() {};