/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   GET.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blarger <blarger@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/09 11:49:01 by blarger           #+#    #+#             */
/*   Updated: 2024/09/03 11:45:47 by blarger          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "GET.hpp"
#include "core.hpp"

std::vector<char> GET::getResponseAtLocation(Webserv &webserv,
                                             ClientRequest &req, int &clientFD)
{
  std::string URI = req.URIpath;
  std::vector<char> response;
  std::map<std::string, LocationConfig> locations = serverConfig.locations;
  std::map<std::string, LocationConfig>::const_iterator it;

  findURIstartInLocations(req.URIpath, locations, it);

  if (it != locations.end())
  {
    std::string path = req.pathOnServer;

    if (it->first == "/delete")
      return (composeOkHtmlResponse(manageDeleteEndPoint(), req.buffer, webserv.sessions, req));
    // Check if the location has a redirection
    else if (it->second.redirection.first)
    {
      response = createRedirectResponse(it->second.redirection.first,
                                        it->second.redirection.second,
																				req.buffer, webserv.sessions,
																				req);
      return (response);
    }
    // if doesn't exist as a file or folder on the server
    else if (!isDirectory(path) && !isFile(path))
      throw HttpException(404, "pathOnServer not a file or folder on server");
    else if (isFile(path))
    {
      std::string fileName, extension;
      getFileNameAndExtension(path, fileName, extension);

      // if file is html
      if (extension == "html")
      {
        response = composeOkHtmlResponse(extractHtmlContentFromFile(path),
                                         req.buffer, webserv.sessions, req);
      }
      // if file is a script in php or python
      else if (extension == "php" || extension == "py")
        webserv.executeScript(path, extension, req.queryString, clientFD);
      // other files
      else if (extension.size())
      {
        std::vector<char> fileContent = readFile(path);
        if (fileContent.empty())
          throw HttpException(404, "File to read not found.");
        response = composeFileResponse(fileContent, URI, req.buffer, webserv.sessions,
																				req);
      }
      return (response);
    }
    // Check if the location has a file to serve
    else if (!it->second.index.empty())
    {
      path += "/" + it->second.index;
      response
          = composeOkHtmlResponse(extractHtmlContentFromFile(path), req.buffer, webserv.sessions, req);
      return (response);
    }
    // If location doesn't have a file, is a folder, and autoindex on
    else if (it->second.index.empty() && isDirectory(path)
             && it->second.autoIndexOn)
    {
      std::vector<std::string> contents = listDirectoryContent(path);
      response = composeOkHtmlResponse(generateDirectoryListing(path, contents),
	                                      req.buffer, webserv.sessions, req);
      return (response);
    }
    else
    {
      throw HttpException(
          403, "You don't have permission to access this directory: " + path);
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
    std::vector<char> response
        = getResponseAtLocation(webserv, client.req, client.socketFD);

    size_t i = webserv.findClientIndexFromFD(clientFD);
    webserv.clients[i].response.insert(webserv.clients[i].response.begin(),
                                       response.begin(), response.end());
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