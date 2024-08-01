/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Webserv.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: demre <demre@student.42malaga.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/31 18:01:57 by demre             #+#    #+#             */
/*   Updated: 2024/07/31 18:02:05 by demre            ###   ########.fr       */
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
      // Close connection and remove from pollfd array
      closeConnection(i);
      --i;
    }
  }
  else if (bytes_read == 0)
  {
    std::cout << "Connection closed: " << fds[i].fd << std::endl;
    // Close connection and remove from pollfd array
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
    clientBuffer += buffer; // buffers[i] == staticBuffer

    if (!strncmp("GET ", clientBuffer.c_str(), 4))
      GET method(serverIndex, fds[i].fd, clients[i].buffer);
    // else if (!strncmp("PUT ", buffers[i].c_str(), 4))
    //   processPutMethod(serverFD, fds[i].fd);
    // else if (!strncmp("DELETE ", buffers[i].c_str(), 7))
    //   processDeleteMethod(serverFD, fds[i].fd);
    else if (!strncmp("POST ", clientBuffer.c_str(), 5))
      POST method(serverIndex, fds[i].fd, clientBuffer);
    else
    {
      std::cout << RED << "Unknown instruction received!"
                << " staticBuffer = " << clientBuffer << RESET << std::endl;
      clientBuffer.erase();
    }
  }
}
