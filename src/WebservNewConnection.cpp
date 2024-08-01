/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   WebservNewConnection.cpp                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: demre <demre@student.42malaga.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/01 20:07:01 by demre             #+#    #+#             */
/*   Updated: 2024/08/01 20:07:01 by demre            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Webserv.hpp"

void Webserv::handleNewConnection(
    size_t i, const std::vector<ServerConfig> &serverConfigs)
{
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

    if (setNonBlocking(newSocket) < 0)
    {
      close(newSocket);
      perror("Failed to set non-blocking for new connection");
      continue;
    }

    std::cout << "New connection accepted: " << newSocket << ", on port: "
              << serverConfigs[clients[i].serverIndex].getPort() << std::endl;

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
    clients.push_back(ci);
  }
}
