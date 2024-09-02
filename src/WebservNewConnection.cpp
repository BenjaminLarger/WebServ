/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   WebservNewConnection.cpp                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: demre <demre@student.42malaga.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/01 20:07:01 by demre             #+#    #+#             */
/*   Updated: 2024/09/02 13:14:20 by demre            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Webserv.hpp"
#include "core.hpp"

void Webserv::handleNewConnection(size_t &i)
{
  while (true)
  {
    int newSocket = accept(fds[i].fd, NULL, NULL);
    if (newSocket < 0)
    {
      if (errno == EAGAIN || errno == EWOULDBLOCK)
        break; // No more incoming connections at the moment
      else     // Error accepting new connection
        std::cerr << "Failed to accept new connection." << std::endl;
    }

    if (setNonBlocking(newSocket) < 0)
    {
      close(newSocket);
      std::cerr << "Failed to set new connection as non-blocking." << std::endl;
    }

    std::cout << "New connection accepted: " << newSocket
              << ", on port: " << clients[i].port << std::endl;

    // Add the new socket to the pollfd vector
    pollfd pfd;
    pfd.fd = newSocket;
    pfd.events = (POLLIN | POLLOUT);
    pfd.revents = 0;
    fds.push_back(pfd);

    // Add the new client info to the clients vector
    ClientInfo ci;
    ci.socketFD = newSocket;
    ci.port = clients[i].port;
    clients.push_back(ci);
  }
}
