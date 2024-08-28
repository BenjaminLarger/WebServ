/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   WebservClientRequest.cpp                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isporras <isporras@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/01 20:07:09 by demre             #+#    #+#             */
/*   Updated: 2024/08/28 15:00:14 by isporras         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "DELETE.hpp"
#include "GET.hpp"
#include "POST.hpp"
#include "Webserv.hpp"

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
  std::string reqLoc = client.req.URIpath;
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
  std::string reqHost = !client.req.fields["Host"].empty()
                            ? client.req.fields["Host"]
                            : client.req.fields["host"];

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
	(void) index;
  char tempBuffer[BUFFER_SIZE];
  ssize_t totalBytesReceived = 0;
  ssize_t bytesReceived;
  contentLength = 0;

   bytesReceived = recv(sockfd, tempBuffer, BUFFER_SIZE - 1, 0);
    if (bytesReceived == -1)
    {
      throw HttpException(400, strerror(errno));
    }
    else if (bytesReceived == 0)
    {
      // Connection closed
//      break;
    }
    else
    {
      tempBuffer[bytesReceived] = '\0';
      buffer.insert(buffer.end(), tempBuffer, tempBuffer + bytesReceived);
      totalBytesReceived += bytesReceived;
      std::cout << "bytes received = " << bytesReceived
                << "\n temp buffer = " << tempBuffer << std::endl;
			std::cout << RED << "Total bytes received : " << totalBytesReceived / (1024.0 * 1024.0) << " MB" << RESET << std::endl;
    }
  // std::cout << buffer << std::endl;
  return (totalBytesReceived);
}
void readClientInput(const std::vector<char> &clientInput)
{
  std::string str(clientInput.begin(), clientInput.end());
  std::cout << BLUE << str << RESET << std::endl;
}

void Webserv::handleClientRequest(
    size_t &i, const std::vector<ServerConfig> &serverConfigs)
{
  std::vector<char> buffer;

  ClientInfo &client = clients[i];
  try
  {
    ssize_t bytesRead = recvAll(fds[i].fd, buffer);
    if (bytesRead < 0)
    {
      if (errno != EAGAIN && errno != EWOULDBLOCK)
      {
        closeConnection(i);
        --i;
        throw HttpException(500, strerror(errno));
      }
    }
    else if (bytesRead == 0)
    {
      closeConnection(i);
      --i;
    }
/* 		else if (bytesRead > serverConfigs[0].maxBodySize)
			throw (HttpException(413, "Payload too large")); */
    else
    {
      int clientFD = client.socketFD; // required in case CGI script

      std::vector<char> clientInput(client.req.buffer.begin(),
                                    client.req.buffer.end());
      //std::cout << GREEN << client.req.buffer << RESET << std::endl;
      readClientInput(clientInput);
      clientInput.insert(clientInput.end(), buffer.begin(), buffer.end());
      std::string clientStr(clientInput.begin(), clientInput.end());
      client.req.buffer = clientStr;
      //std::cout << MAGENTA << clientStr << RESET << std::endl;
      //std::cout << CYAN << client.req.buffer << RESET << std::endl;
      //std::cout << RED << "boundary = " << boundary << RESET << std::endl;

      parseClientRequest(client.req, serverConfigs[0].maxBodySize);

			if (hasBlankLineInput(client.req.buffer, boundary, client) == true)
      {
        std::cout << "Request received on port " << client.port
                  << ", client.socketFD " << client.socketFD
                  << ", root: " << client.client_serverConfig.serverRoot
                  << std::endl;

        displayParsedHeaderRequest(client);
				
        // Looks for the serverConfig that matches the Host value of the request
        client.client_serverConfig
            = findClientServerConfig(client, serverConfigs);

        resolveRequestedPathFromLocations(client.req,
                                          client.client_serverConfig);

        displayClientRequestLocationData(client);

        if (isMethodAllowedAtLoc(client.req, client.client_serverConfig))
        {
          if (client.req.method == "GET")
            GET method(*this, client, client.client_serverConfig);
          else if (client.req.method == "POST")
							POST method(*this, client, fds[i].fd, clientInput,
													client.client_serverConfig, boundary);
          else if (client.req.method == "DELETE")
            DELETE method(client, client.client_serverConfig);
					
          std::cout << RED << "Clearing request buffers. Method well proceeded" << RESET << std::endl;

          // if the method is a CGI script, we are adding the pipe to pollfd and clients vectors, so we need to get the index again to have the correct client.

          size_t j = findClientIndexFromFD(clientFD);
          clients[j].req.buffer.clear();
          clientInput.clear();
          clientStr.clear();
          buffer.clear();
        }
        else
        {
          std::cerr << RED << "Clearing request buffers" << RESET << std::endl;
          size_t j = findClientIndexFromFD(clientFD);
          clients[j].req.buffer.clear();
          clientInput.clear();
          clientStr.clear();
          buffer.clear();
          throw HttpException(405, "Method is not allowed on that path");
        }
      }
    }
  }
  catch (const HttpException &e)
  {
    std::cerr << RED << "Error: " << e.getStatusCode() << " " << e.what()
              << RESET << '\n';

    clients[i].response = composeErrorHtmlPage(
		e.getStatusCode(), getReasonPhrase(e.getStatusCode()),
		clients[i].client_serverConfig.errorPages);
    clients[i].totalToSend = clients[i].response.size();
    clients[i].totalBytesSent = 0;
  }
}
