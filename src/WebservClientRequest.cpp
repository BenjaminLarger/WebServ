/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   WebservClientRequest.cpp                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isporras <isporras@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/01 20:07:09 by demre             #+#    #+#             */
/*   Updated: 2024/08/10 16:52:12 by isporras         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Webserv.hpp"
#include "DELETE.hpp"
#include "GET.hpp"
#include "POST.hpp"

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

ssize_t Webserv::recvAll(int sockfd, std::string &buffer)
{
  char tempBuffer[BUFFER_SIZE];
  ssize_t totalBytesReceived = 0;
  ssize_t bytesReceived;
  contentLength = 0;
  bool hasContentLength = false;

  while (true)
  {
    bytesReceived = recv(sockfd, tempBuffer, BUFFER_SIZE - 1, 0);
    if (bytesReceived == -1)
    {
      // Handle error
      throw HttpException(400, "Bad request");
      break;
    }
    else if (bytesReceived == 0)
    {
      // Connection closed
      break;
    }
    else
    {
      tempBuffer[bytesReceived] = '\0';
      buffer.append(tempBuffer, bytesReceived);
      totalBytesReceived += bytesReceived;
      // Check if the HTTP request is complete
      if (buffer.find("Content-Length:") != std::string::npos)
      {
        size_t startPos = buffer.find("Content-Length:")
                          + std::string("Content-Length:").length();
        size_t endPos = buffer.find("\r\n", startPos);
        std::string contentLengthStr
            = buffer.substr(startPos, endPos - startPos);
        std::cout << RED << "content length str = " << contentLengthStr << RESET
                  << std::endl;
        contentLengthStr.erase(0, contentLengthStr.find_first_not_of(
                                      " \t")); // Trim leading whitespace
        std::istringstream(contentLengthStr) >> contentLength;
        std::cout << GREEN << "Content length found : " << contentLength
                  << RESET << std::endl;
        hasContentLength = true;
      }
      if (hasContentLength == false
          && buffer.find("\r\n\r\n") != std::string::npos)
      {
        // We have received the end of the headers
        break;
      }
      else if (hasContentLength == true && totalBytesReceived >= contentLength)
      {
        //We have read all the content specified by content length
        std::cout << GREEN << "All the request content has been read\n"
                  << RESET;
        break;
      }
    }
  }
  std::cout << buffer << std::endl;
  return totalBytesReceived;
}



void Webserv::handleClientRequest(
    size_t i, const std::vector<ServerConfig> &serverConfigs)
{
  //char buffer[100000];
  std::string buffer;

  try
  {
    //ssize_t bytesRead = read(fds[i].fd, buffer, sizeof(buffer));
    ssize_t bytesRead = recvAll(fds[i].fd, buffer);
    if (bytesRead < 0)
    {
      if (errno != EAGAIN && errno != EWOULDBLOCK)
      {
        closeConnection(i);
        --i;
        throw HttpException(
            500, "Internal Server Error: Data failed to be sent to the client");
      }
    }
    else if (bytesRead == 0)
    {
      closeConnection(i);
      --i;
    }
    else
    {
      // Handle incoming data (e.g., parse HTTP request)
      //buffer[bytesRead] = '\0';
      ClientInfo &client = clients[i];
      size_t &serverIndex = client.serverIndex;
      std::string &clientInput = client.req.buffer;
      const ServerConfig &serverConfig = serverConfigs[serverIndex];

      std::cout << "Request received on serverIndex " << serverIndex
                << ", port " << client.port << ", client.socketFD "
                << client.socketFD << ", root: " << serverConfig.serverRoot
                << std::endl;

      clientInput += buffer;

      parseClientRequest(client.req);

      {
        // Display parsed header request
        std::cout << "Parsed request header: \n"
                  << MAGENTA << client.req.method << " "
                  << client.req.pathToRessource << " " << client.req.HTTPversion
                  << std::endl;
        for (std::map<std::string, std::string>::iterator it
             = client.req.fields.begin();
             it != client.req.fields.end(); it++)
        {
          std::cout << it->first << ": " << it->second << std::endl;
        }
        std::cout << RESET << std::endl;
      }

      handleLocations(client.req, serverConfig);

      if (client.req.method == "GET"
          /* && isMethodAllowedAtLoc("GET", client.req, serverConfig) */)
        GET method(client, fds[i].fd, clientInput, serverConfig);
      else if (client.req.method == "POST")
        POST method(client, serverIndex, fds[i].fd, clientInput, serverConfig);
      else if (client.req.method == "DELETE")
        DELETE method(client, fds[i].fd, clientInput, serverConfig);
      else
      {
        clientInput.erase();
        throw HttpException(405, "Method is not allowed on that path");
      }
      // closeConnection(i); // commented while testing
      // --i; // commented while testing
    }
    // cleanupClientRequest()
  }
  catch (const HttpException &e)
  {
    std::cerr << RED << "Error: " << e.getStatusCode() << " " << e.what()
              << RESET << '\n';

    sendDefaultErrorPage(fds[i].fd, e.getStatusCode(),
                         getReasonPhrase(e.getStatusCode()),
                         serverConfigs[clients[i].serverIndex].errorPages);
  }
}
