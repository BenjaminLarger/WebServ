/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Webserv.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: demre <demre@student.42malaga.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/03 10:06:22 by blarger           #+#    #+#             */
/*   Updated: 2024/07/30 14:12:08 by demre            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Webserv.hpp"

/* --------------CONSTRUCTORS */
Webserv::Webserv(const char *_filename, ServerConfig config)
    : filename(_filename),
      /* config(filename) */ port(config.getPort()),
      serverFD(socket(AF_INET, SOCK_STREAM, 0)), optval(1)
{
  std::ifstream file(filename);

  // countAndParseServer(filename);

  /* for (unsigned int i = 0; i < this->numberOfServers; i++)
  {
  } */
  if (serverFD < 0)
  {
    throw(std::range_error("Failed to create socket."));
  }
  if (setNonBlocking(serverFD) < 0)
  {
    close(serverFD);
    throw(std::range_error("Failed to set non blocking."));
  }

  struct sockaddr_in address;
  address.sin_family = AF_INET;
  address.sin_addr.s_addr = INADDR_ANY;
  address.sin_port = htons(port);

  if (setsockopt(serverFD, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval))
      < 0)
    throw(std::range_error("setsockopt(SO_REUSEADDR) failed"));

  if (bind(serverFD, (struct sockaddr *)&address, sizeof(address)) < 0)
  {
    close(serverFD);
    throw(std::range_error("Failed to bind to port."));
  }

  if (listen(serverFD, 10) < 0)
  {
    close(serverFD);
    throw(std::range_error("Failed to listen on socket."));
  }

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
