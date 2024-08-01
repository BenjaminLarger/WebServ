/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   WebservClientRequest.cpp                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: demre <demre@student.42malaga.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/01 20:07:09 by demre             #+#    #+#             */
/*   Updated: 2024/08/01 20:07:11 by demre            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "GET.hpp"
#include "POST.hpp"
#include "Webserv.hpp"

void Webserv::handleClientRequest(
    size_t i, const std::vector<ServerConfig> &serverConfigs)
{
  char buffer[1024];
  ssize_t bytes_read = read(fds[i].fd, buffer, sizeof(buffer));
  if (bytes_read < 0)
  {
    if (errno != EAGAIN && errno != EWOULDBLOCK)
    {
      perror("Failed to read from client.");
      closeConnection(i);
      --i;
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
    buffer[bytes_read] = '\0';
    size_t serverIndex = clients[i].serverIndex;

    std::cout << "Received on serverIndex " << serverIndex << ", port "
              << serverConfigs[serverIndex].getPort()
              << ", clients[i].socketFD " << clients[i].socketFD << ": "
              << buffer << std::endl;

    std::string &clientBuffer = clients[i].buffer;
    clientBuffer += buffer;

    if (!strncmp("GET ", clientBuffer.c_str(), 4))
      GET method(serverIndex, fds[i].fd, clients[i].buffer);
    // else if (!strncmp("DELETE ", clientBuffer.c_str(), 7))
    //   processDeleteMethod(serverFD, fds[i].fd); ====> to implement
    else if (!strncmp("POST ", clientBuffer.c_str(), 5))
      POST method(serverIndex, fds[i].fd, clientBuffer);
    else
    {
      // Send HTTP/1.0 501 Not Implemented or HTTP/1.0 400 Bad Request

      std::cout << RED << "Unknown instruction received!"
                << " clientBuffer = " << clientBuffer << RESET << std::endl;
      clientBuffer.erase();

      // closeConnection(i); // commented while testing
      // --i; // commented while testing
    }
  }
}
