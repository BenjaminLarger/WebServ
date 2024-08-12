/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   WebservClientRequest.cpp                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: demre <demre@student.42malaga.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/01 20:07:09 by demre             #+#    #+#             */
/*   Updated: 2024/08/12 20:56:15 by demre            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "DELETE.hpp"
#include "GET.hpp"
#include "POST.hpp"
#include "Webserv.hpp"

/* ssize_t read_all(int fd, char *buffer, size_t buffer_size)
{
    ssize_t total_bytesRead = 0;
    ssize_t bytesRead;

    while (total_bytesRead < buffer_size) {
        bytesRead = read(fd, buffer + total_bytesRead, buffer_size - total_bytesRead);
        if (bytesRead == 0) {
            // End of file
            break;
        }
        if (bytesRead == -1) {
            if (errno == EINTR) {
                // Interrupted by signal, continue reading
                continue;
            } else if (errno == EAGAIN || errno == EWOULDBLOCK) {
                // Non-blocking mode and no data available, handle accordingly
                break;
                           // Other errors
                std::cerr << "Read error: " << strerror(errno) << std::endl;
                return -1;
            }
        total_bytesRead += bytesRead;
    }

    return total_bytesRead;
} */

//Check if reqHost is inside the serverNames vector of the serverConfig
bool checkReqHostInServerNames(std::string reqHost,
                               std::vector<std::string> serverNames, int port)
{
  std::ostringstream oss;
  oss << ":";
  oss << port;

  for (size_t i = 0; i < serverNames.size(); i++)
  {
    serverNames[i] += oss.str();
    if (reqHost == serverNames[i])
      return (true);
  }

  return (false);
}

bool checkIdenticalHostPort(const std::vector<ServerConfig> &serverConfigs,
                            size_t i, std::string reqHost)
{
  if (i + 1 < serverConfigs.size())
  {
    for (std::vector<ServerConfig>::const_iterator it
         = serverConfigs.begin() + i + 1;
         it != serverConfigs.end(); it++)
    {
      if (it->getHost() == serverConfigs[i].getHost()
          && it->getPort() == serverConfigs[i].getPort()
          && checkReqHostInServerNames(reqHost, it->serverNames, it->getPort()))
        return (true);
    }
  }
  return (false);
}

// Decide which serverConfig to look in depending on the location
const ServerConfig &findClientServerConfigByLoc(
    ClientInfo &client, const std::vector<ServerConfig> &serverConfigs)
{
  std::string reqLoc = client.req.URI;
  size_t firstPos = reqLoc.find('/');
  size_t secondPos = std::string::npos;

  if (firstPos != std::string::npos)
    secondPos = reqLoc.find('/', firstPos + 1);

  std::string baseLoc
      = (secondPos != std::string::npos) ? reqLoc.substr(0, secondPos) : reqLoc;
  if (baseLoc.empty())
    baseLoc = reqLoc;

  for (size_t i = 0; i < serverConfigs.size(); i++)
  {
    std::map<std::string, LocationConfig>::const_iterator it
        = serverConfigs[i].locations.find(baseLoc);
    if (it != serverConfigs[i].locations.end())
    {
      std::cout << "The key '" << baseLoc
                << "' exist in the map with value: " << std::endl;
      return (serverConfigs[i]);
    }
    else
      std::cout << "The key '" << baseLoc << "' don't exist in the map."
                << std::endl;
  }

  // If no server name matches the request host, return the first server config that has that ip and port
  throw HttpException(404, "No server found for the request host");
}

const ServerConfig &findClientServerConfig(
    ClientInfo &client, const std::vector<ServerConfig> &serverConfigs)
{
  std::string serverName;
  std::string reqHost = client.req.fields["Host"];

  for (size_t i = 0; i < serverConfigs.size(); i++)
  {
    std::ostringstream oss;
    oss << ":";
    oss << serverConfigs[i].getPort();
    for (size_t j = 0; j < serverConfigs[i].serverNames.size(); j++)
    {
      serverName = serverConfigs[i].serverNames[j] + oss.str();
      std::cout << "reqHost = '" << reqHost << "'" << std::endl;
      std::cout << "serverName = '" << serverName << "'" << std::endl;
      if (serverName == reqHost
          && checkIdenticalHostPort(serverConfigs, i, reqHost))
      {
        //In case any location match the request URI we return the first serverConfig that has the same host and port
        client.client_serverConfig = serverConfigs[i];
        std::cout << "Looking for location to set serverConfig " << i
                  << std::endl;
        return (findClientServerConfigByLoc(client, serverConfigs));
      }
      else if (serverName == reqHost
               && serverConfigs[i].getPort() == client.port)
        return (serverConfigs[i]);
    }
    //Reset oss
    oss.str("");
  }
  // If no server name matches the request host, return the first server config by default
  throw HttpException(404, "No server found for the request host");
}

ssize_t Webserv::recvAll(int sockfd, std::vector<char> &buffer)
{
  char tempBuffer[BUFFER_SIZE];
  ssize_t totalBytesReceived = 0;
  ssize_t bytesReceived;
  contentLength = 0;
  bool hasContentLength = false;

  while (true)
  {
    std::cerr << RED << "recvAll 1" << RESET << '\n';
    bytesReceived = recv(sockfd, tempBuffer, BUFFER_SIZE - 1, 0);
    if (bytesReceived == -1)
    {
      std::cerr << RED << "recvAll 2" << RESET << '\n';
      if (errno == EAGAIN || errno == EWOULDBLOCK)
      {
        std::cerr << RED << "recvAll 3" << RESET << '\n';
        std::cout << "HERE!\n";
        // Resource temporarily unavailable, retry the recv call
        break;
      }
      // Handle error
      throw HttpException(400, strerror(errno));
    }
    else if (bytesReceived == 0)
    {
      std::cerr << RED << "recvAll 4" << RESET << '\n';
      // Connection closed
      break;
    }
    else
    {
      std::cerr << RED << "recvAll 5" << RESET << '\n';
      tempBuffer[bytesReceived] = '\0';
      buffer.insert(buffer.end(), tempBuffer, tempBuffer + bytesReceived);
      totalBytesReceived += bytesReceived;
      std::cout << "bytes received = " << bytesReceived
                << "\n temp buffer = " << tempBuffer << std::endl;
      if (hasContentLength == false
          && std::string(buffer.begin(), buffer.end()).find("\r\n\r\n")
                 != std::string::npos)
      {
        // We have received the end of the headers
        break;
      }
    }
  }
  // std::cout << buffer << std::endl;
  return (totalBytesReceived);
}

void Webserv::handleClientRequest(
    size_t i, const std::vector<ServerConfig> &serverConfigs)
{
  //char buffer[100000];
  std::vector<char> buffer;

  try
  {
    std::cerr << RED << "handleClientRequest 1" << RESET << '\n';
    std::cerr << RED << "fds[i].fd: " << fds[i].fd << RESET << '\n';
    ssize_t bytesRead = recvAll(fds[i].fd, buffer);
    if (bytesRead < 0)
    {
      std::cerr << RED << "handleClientRequest 2" << RESET << '\n';
      if (errno != EAGAIN && errno != EWOULDBLOCK)
      {
        closeConnection(i);
        --i;
        throw HttpException(500, strerror(errno));
      }
    }
    else if (bytesRead == 0)
    {
      std::cerr << RED << "handleClientRequest 3" << RESET << '\n';
      closeConnection(i);
      --i;
    }
    else
    {
      std::cerr << RED << "handleClientRequest 4" << RESET << '\n';
      ClientInfo &client = clients[i];
      //std::vector<char> &clientInput += client.req.buffer;
      std::cerr << RED << "handleClientRequest 5" << RESET << '\n';
      std::vector<char> clientInput(client.req.buffer.begin(),
                                    client.req.buffer.end());
      std::cerr << RED << "handleClientRequest 6" << RESET << '\n';

      clientInput.insert(clientInput.end(), buffer.begin(), buffer.end());
      std::string clientStr(clientInput.begin(), clientInput.end());
      client.req.buffer = clientStr;
      if (hasBlankLineInput(client.req.buffer) == true)
      {
        parseClientRequest(client.req);
        //Looks for the serverConfig that matches the Host value of the request
        client.client_serverConfig
            = findClientServerConfig(client, serverConfigs);
        std::cout << "Request received on port " << client.port
                  << ", client.socketFD " << client.socketFD
                  << ", root: " << client.client_serverConfig.serverRoot
                  << std::endl;

        {
          // Display parsed header request
          std::cout << "Parsed request header: \n"
                    << MAGENTA << client.req.method << " " << client.req.URI
                    << " " << client.req.HTTPversion << std::endl;
          for (std::map<std::string, std::string>::iterator it
               = client.req.fields.begin();
               it != client.req.fields.end(); it++)
          {
            std::cout << it->first << ": " << it->second << std::endl;
          }
          std::cout << RESET << std::endl;
        }

        resolveRequestedPathFromLocations(client.req,
                                          client.client_serverConfig);

        {
          // Display client request location data
          std::cout << "URI: " << client.req.URI
                    << ", pathFolder: " << client.req.pathFolder
                    << ", pathOnServer: " << client.req.pathOnServer
                    << ", isDir " << isDirectory(client.req.pathOnServer)
                    << ", isFile " << isFile(client.req.pathOnServer)
                    << ", pathFolderOnServer: " << client.req.pathFolderOnServer
                    << ", isDir " << isDirectory(client.req.pathFolderOnServer)
                    << ", isFile " << isFile(client.req.pathFolderOnServer)
                    << std::endl;
        }

        if (isMethodAllowedAtLoc(client.req, client.client_serverConfig))
        {
          if (client.req.method == "GET")
            GET method(*this, client, fds[i].fd, clientStr,
                       client.client_serverConfig);
          else if (client.req.method == "POST")
            POST method(client, fds[i].fd, clientInput,
                        client.client_serverConfig);
          else if (client.req.method == "DELETE")
            DELETE method(client, fds[i].fd, clientStr,
                          client.client_serverConfig);
        }
        else
        {
          clientInput.clear();
          throw HttpException(405, "Method is not allowed on that path");
        }
      }
    }
  }
  catch (const HttpException &e)
  {
    std::cerr << RED << "Error: " << e.getStatusCode() << " " << e.what()
              << RESET << '\n';
    std::cout << "Sending default error page\n";
    sendDefaultErrorPage(fds[i].fd, e.getStatusCode(),
                         getReasonPhrase(e.getStatusCode()),
                         clients[i].client_serverConfig.errorPages);
  }
}
