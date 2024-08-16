/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   WebservNewConnection.cpp                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blarger <blarger@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/01 20:07:01 by demre             #+#    #+#             */
/*   Updated: 2024/08/15 13:03:43 by blarger          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Webserv.hpp"
#include "core.hpp"

void Webserv::handleNewConnection(
    size_t &i, const std::vector<ServerConfig> &serverConfigs)
{
  (void)serverConfigs;
  while (true)
  {
    int newSocket = accept(fds[i].fd, NULL, NULL);
    if (newSocket < 0)
    {
      if (errno == EAGAIN || errno == EWOULDBLOCK)
      {
        // No more incoming connections at the moment
        break;
      }
      else
      {
        // Error accepting new connection
        throw HttpException(
            500, "Internal Server Error: Failed to accept new connection");
        break;
      }
    }
    if (setNonBlocking(newSocket) < 0)
    {
      close(newSocket);
      throw HttpException(500, "Internal Server Error: Data failed to be sent "
                               "to the client (socket)");
    }
    std::cout << "New connection accepted: " << newSocket
              << ", on port: " << clients[i].port << std::endl;

    // Add the new socket to the pollfd vector
    pollfd pfd;
    pfd.fd = newSocket;
    pfd.events |= (POLLIN | POLLOUT);
    pfd.revents = 0;
    fds.push_back(pfd);

    // Add the new client info to the clients vector
    ClientInfo ci;
    ci.socketFD = newSocket;
    ci.port = clients[i].port;
    clients.push_back(ci);
  }
}
