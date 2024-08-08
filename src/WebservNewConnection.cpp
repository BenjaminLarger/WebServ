/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   WebservNewConnection.cpp                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: demre <demre@student.42malaga.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/01 20:07:01 by demre             #+#    #+#             */
/*   Updated: 2024/08/08 13:59:31 by demre            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Webserv.hpp"
#include "core.hpp"

void Webserv::handleNewConnection(
    size_t i, const std::vector<ServerConfig> &serverConfigs)
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
        std::cerr << "accept failed: " << strerror(errno)
                  << " (errno: " << errno << ")" << std::endl;
        break;
      }
    }
    try
    {
      if (setNonBlocking(newSocket) < 0)
      {
        close(newSocket);
        throw HttpException(
            500, "Internal Server Error: Data failed to be sent to the client");
      }
      std::cout << "New connection accepted: " << newSocket
                << ", on port: " << clients[i].port << std::endl;

      // Add the new socket to the pollfd vector
      pollfd pfd;
      pfd.fd = newSocket;
      pfd.events = POLLIN;
      pfd.revents = 0;
      fds.push_back(pfd);

      // Add the new client info to the clients vector
      ClientInfo ci;
      ci.socketFD = newSocket;
      ci.serverIndex = clients[i].serverIndex;
      ci.port = clients[i].port;
      clients.push_back(ci);
    }
    catch (const HttpException &e)
    {
      std::cerr << "Error: " << e.what() << '\n';
      continue;
    }
  }
}
