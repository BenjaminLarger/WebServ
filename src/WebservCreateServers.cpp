/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   WebservCreateServers.cpp                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: demre <demre@student.42malaga.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/01 11:06:41 by demre             #+#    #+#             */
/*   Updated: 2024/08/02 13:42:27 by demre            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Webserv.hpp"

// Create and set up listening sockets (serverFD) for all servers on all ports and add them to std::vector<pollfd> fds
void Webserv::createServers(std::vector<ServerConfig> &serverConfigs)
{
  for (std::vector<ServerConfig>::size_type i = 0; i < serverConfigs.size();
       ++i)
  {
    // Create socket and check if successful
    int serverFD = socket(AF_INET, SOCK_STREAM, 0);
    if (serverFD < 0)
      throw(std::range_error("Failed to create socket."));

    // serverFds.push_back(serverFD); // don't know if needed

    // Set the socket to non-blocking mode
    if (setNonBlocking(serverFD) < 0)
    {
      close(serverFD);
      throw(std::range_error("Failed to set non-blocking."));
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
      throw(std::range_error("Invalid address/ Address not supported."));
    }

    serverAddress.sin_port = htons(serverConfigs[i].getPort());

    // Set socket options to allow reuse of local addresses
    int optval = 1;
    if (setsockopt(serverFD, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval))
        < 0)
    {
      close(serverFD);
      throw(std::range_error("setsockopt(SO_REUSEADDR) failed"));
    }

    // Link the socket to the specified address and port
    if (bind(serverFD, (struct sockaddr *)&serverAddress, sizeof(serverAddress))
        < 0)
    {
      close(serverFD);
      throw(std::range_error("Failed to bind to port."));
    }

    // Start listening for incoming connections on the socket
    if (listen(serverFD, 10) < 0)
    {
      close(serverFD);
      throw(std::range_error("Failed to listen on socket."));
    }

    // Add the listening socket to the pollfd vector
    pollfd pfd;
    pfd.fd = serverFD;
    pfd.events = POLLIN | POLLOUT; // Monitor both read and write events
    pfd.revents = 0;
    fds.push_back(pfd);

    // Add a dummy client info for the listening socket
    ClientInfo ci;
    ci.socketFD = serverFD;
    ci.serverIndex = i; // Associate with the respective server
    ci.port = serverConfigs[i].getPort();
    clients.push_back(ci);
  }
}