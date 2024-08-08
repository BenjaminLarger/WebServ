/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   WebservClientRequest.cpp                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blarger <blarger@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/01 20:07:09 by demre             #+#    #+#             */
/*   Updated: 2024/08/08 13:17:57 by blarger          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "GET.hpp"
#include "POST.hpp"
#include "Webserv.hpp"

/* ssize_t read_all(int fd, char *buffer, size_t buffer_size)
{
    ssize_t total_bytes_read = 0;
    ssize_t bytes_read;

    while (total_bytes_read < buffer_size) {
        bytes_read = read(fd, buffer + total_bytes_read, buffer_size - total_bytes_read);
        if (bytes_read == 0) {
            // End of file
            break;
        }
        if (bytes_read == -1) {
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
        total_bytes_read += bytes_read;
    }

    return total_bytes_read;
} */

ssize_t Webserv::recvAll(int sockfd, std::string &buffer)
{
  char tempBuffer[BUFFER_SIZE];
  ssize_t totalBytesReceived = 0;
  ssize_t bytesReceived;
	contentLength = 0;
	bool	hasContentLength = false;

  while (true) {
      bytesReceived = recv(sockfd, tempBuffer, BUFFER_SIZE - 1, 0);
      if (bytesReceived == -1) {
          // Handle error
          perror("recv");
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
					size_t startPos = buffer.find("Content-Length:") + std::string("Content-Length:").length();
					size_t endPos = buffer.find("\r\n", startPos);
					std::string contentLengthStr = buffer.substr(startPos, endPos - startPos);
					std::cout << RED << "content length str = " << contentLengthStr << RESET << std::endl;
					contentLengthStr.erase(0, contentLengthStr.find_first_not_of(" \t")); // Trim leading whitespace
					std::istringstream(contentLengthStr) >> contentLength;
					std::cout << GREEN << "Content length found : " << contentLength << RESET << std::endl;
					hasContentLength = true;
				}
        if (hasContentLength == false && buffer.find("\r\n\r\n") != std::string::npos)
				{
          // We have received the end of the headers
          break;
        }
				else if (hasContentLength == true &&  totalBytesReceived >= contentLength)
				{
					//We have read all the content specified by content length
					std::cout << GREEN << "All the request content has been readen!\n" << RESET;
					break ;
				}
      }
    }
		std::cout << buffer << std::endl;
    return totalBytesReceived;
}

void Webserv::handleClientRequest(
    size_t i, const std::vector<ServerConfig> &serverConfigs)
{
  (void)serverConfigs;
  //char buffer[100000];
	std::string buffer;

  try
  {
    //ssize_t bytes_read = read(fds[i].fd, buffer, sizeof(buffer));
		ssize_t bytes_read = recvAll(fds[i].fd, buffer);		
    if (bytes_read < 0)
    {
      if (errno != EAGAIN
          && errno
                 != EWOULDBLOCK) // [Isaac] Need to talk about if this exception could break the logic
      {
        closeConnection(i);
        --i;
        throw HttpException(
            "500",
            "Internal Server Error: Data failed to be sent to the client");
      }
    }
    else if (bytes_read == 0)
    {
      closeConnection(i);
      --i;
    }
    else
    {
      // Handle incoming data (e.g., parse HTTP request)
      //buffer[bytes_read] = '\0';
      size_t serverIndex = clients[i].serverIndex;

      std::cout << "Request received on serverIndex " << serverIndex
                << ", port " << clients[i].port << ", clients[i].socketFD "
                << clients[i].socketFD
                << ", root: " << serverConfigs[serverIndex].serverRoot
                << std::endl;
      // std::cout << "Request: \n" << buffer << std::endl;

      std::string &clientBuffer = clients[i].req.buffer;
      clientBuffer += buffer;

      // parseClientRequest();
      if (!strncmp("GET ", clientBuffer.c_str(), 4))
        GET method(fds[i].fd, clientBuffer, serverConfigs[serverIndex]);
      else if (!strncmp("POST ", clientBuffer.c_str(), 5))
        POST method(serverIndex, fds[i].fd, clientBuffer, serverConfigs[serverIndex]);
      else
      {
        clientBuffer.erase();
        throw HttpException("400", "Bad request");
      }
      // closeConnection(i); // commented while testing
      // --i; // commented while testing
    }
    // cleanupClientRequest()
  }
  catch (const HttpException &e)
  {
    std::cerr << RED << "Error: " << e.what() << RESET << '\n';
    sendDefaultErrorPage(fds[i].fd, e.getStatusCode(), e.getErrorMessage(),
                         serverConfigs[clients[i].serverIndex].errorPages);
  }
}
