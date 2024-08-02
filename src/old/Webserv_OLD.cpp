/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Webserv_OLD.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blarger <blarger@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/03 10:06:22 by blarger           #+#    #+#             */
/*   Updated: 2024/08/01 12:40:38 by blarger          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Webserv.hpp"

/* --------------CONSTRUCTORS */
//The socket() function shall create an unbound socket in a communications domain,
//and return a file descriptor that can be used in later function calls that operate on sockets.

Webserv::Webserv(const char *_filename, ServerConfig config)
    : filename(_filename), port(config.getPort()),
      serverFD(socket(AF_INET, SOCK_STREAM, 0)), optval(1)
{
  // Open the configuration file
  std::ifstream file(filename);

  // countAndParseServer(filename) => pars multiple configto implement, the actual function only parse one configuration file;
  /* for (unsigned int i = 0; i < this->numberOfServers; i++)
  {
  } */

  // Check if socket creation was successful
  if (serverFD < 0)
  {
    throw(std::range_error("Failed to create socket."));
  }

  // Set the socket to non-blocking mode
  if (setNonBlocking(serverFD) < 0)
  {
    close(serverFD);
    throw(std::range_error("Failed to set non blocking."));
  }

  struct sockaddr_in address;
  address.sin_family = AF_INET;
  address.sin_addr.s_addr = INADDR_ANY;
  address.sin_port = htons(port);

  // Set socket options to allow reuse of local addresses
  if (setsockopt(serverFD, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval))
      < 0)
    throw(std::range_error("setsockopt(SO_REUSEADDR) failed"));

  // Link the socket to the specified address and port
  if (bind(serverFD, (struct sockaddr *)&address, sizeof(address)) < 0)
  {
    close(serverFD);
    throw(std::range_error("Failed to bind to port."));
  }

  // Start listening for incoming connections on the socket
  if (listen(serverFD, 10) < 0) // here the second parameter is  how many pending connections you can have before the kernel starts rejecting new ones.
  {
    close(serverFD);
    throw(std::range_error("Failed to listen on socket."));
  }

  // Enter the server's main listening loop to handle incoming connections
  serverListeningLoop(serverFD);
}

Webserv::Webserv(void)
    : filename(DEFAULT_CONFIG_PATH),
      /*  config(filename), */ port(config.getPort()),
      serverFD(socket(AF_INET, SOCK_STREAM, 0)), optval(1)
{
}

/* --------------DESTRUCTORS */
Webserv::~Webserv(void) { close(serverFD); }

/* --------------COPY */
/* Webserv::Webserv(const Webserv &other) : port(other.port),
serverFD(other.serverFD)
{
} */

/* --------------OPERATOR */
const Webserv &Webserv::operator=(const Webserv &other)
{
  if (this != &other)
  {
    port = other.getPort();
    serverFD = other.getServerFD();
  }
  return *this;
}

/* --------------GETTER */
const unsigned int &Webserv::getPort(void) const { return (this->port); }

const int &Webserv::getServerFD(void) const { return (this->serverFD); }
