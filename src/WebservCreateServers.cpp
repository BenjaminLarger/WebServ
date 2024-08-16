/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   WebservCreateServers.cpp                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blarger <blarger@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/01 11:06:41 by demre             #+#    #+#             */
/*   Updated: 2024/08/15 13:03:32 by blarger          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Webserv.hpp"

bool checkIfSameHostAndPort(std::vector<ServerConfig> &serverConfigs, int i,
                            int &fd_pos)
{
  for (int j = 0; j < i; ++j)
  {
    if (serverConfigs[i].getHost() == serverConfigs[j].getHost()
        && serverConfigs[i].getPort() == serverConfigs[j].getPort())
    {
      fd_pos = j;
      return (true);
    }
  }
  return (false);
}

// Create and set up listening sockets (serverFD) for all servers on all ports and add them to std::vector<pollfd> fds
void Webserv::createServers(std::vector<ServerConfig> &serverConfigs)
{
  for (std::vector<ServerConfig>::size_type i = 0; i < serverConfigs.size();
       ++i)
  {
    int fd_pos;

    if (checkIfSameHostAndPort(serverConfigs, i, fd_pos))
    {
      // If the host and port are the same as a previous server,
      pollfd pfd;
      printf("Same host and port as server %d\n", fd_pos);
      pfd.fd = fds[fd_pos].fd;
      pfd.events |= (POLLIN | POLLOUT);
      pfd.revents = 0;
      fds.push_back(pfd);

      // Add a dummy client info for the listening socket
      ClientInfo ci;
      ci.socketFD = fds[fd_pos].fd;
      clients.push_back(ci);
      continue;
    }

    // Create socket and check if successful
    int serverFD = socket(AF_INET, SOCK_STREAM, 0);
    if (serverFD < 0)
      throw HttpException(500, "Failed to create socket.");

    // serverFds.push_back(serverFD); // don't know if needed

    // Set the socket to non-blocking mode
    if (setNonBlocking(serverFD) < 0)
    {
      close(serverFD);
      throw HttpException(500, "Failed to set non-blocking.");
    }

    struct sockaddr_in serverAddress;
    memset(&serverAddress, 0, sizeof(serverAddress)); // not sure if needed
    serverAddress.sin_family = AF_INET;

    // serverAddress.sin_addr.s_addr = INADDR_ANY;
    // Convert the host string to network address using inet_addr
    serverAddress.sin_addr.s_addr
        = inet_addr(serverConfigs[i].getHost().c_str());
    if (serverAddress.sin_addr.s_addr == INADDR_NONE)
    {
      close(serverFD);
      throw HttpException(500, "Invalid address/ Address not supported.");
    }

    serverAddress.sin_port = htons(serverConfigs[i].getPort());

    // Set socket options to allow reuse of local addresses
    int optval = 1;
    if (setsockopt(serverFD, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval))
        < 0)
    {
      close(serverFD);
      throw HttpException(500, "setsockopt(SO_REUSEADDR) failed");
    }

    // Link the socket to the specified address and port
    if (bind(serverFD, (struct sockaddr *)&serverAddress, sizeof(serverAddress))
        < 0)
    {
      close(serverFD);
      throw HttpException(500, "Failed to bind to port.");
    }

    // Start listening for incoming connections on the socket
    if (listen(serverFD, 10) < 0)
    {
      close(serverFD);
      throw HttpException(500, "Failed to listen on socket.");
    }

    // Add the listening socket to the pollfd vector
    pollfd pfd;
    pfd.fd = serverFD;
    pfd.events |= (POLLIN | POLLOUT);
    pfd.revents = 0;
    fds.push_back(pfd);

    // Add a dummy client info for the listening socket
    ClientInfo ci;
    ci.socketFD = serverFD;
    ci.port = serverConfigs[i].getPort();
    clients.push_back(ci);
  }
}