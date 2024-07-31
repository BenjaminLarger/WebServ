/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Webserv.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blarger <blarger@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/09 18:15:50 by demre             #+#    #+#             */
/*   Updated: 2024/07/31 15:31:22 by blarger          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#ifndef WEBSERV_HPP
#define WEBSERV_HPP

#include "ConfigurationFile.hpp"
#include "Dependencies.hpp"

// ************************************************************************** //
//                               Class //
// ************************************************************************** //

class Webserv : public ServerConfig
{
private:
  const char *filename;
  ServerConfig config;
  unsigned int port;
  int serverFD;
  int optval;
  unsigned int numberOfServers;

public:
  Webserv();
  Webserv(const char *filename, ServerConfig config);
  ~Webserv(void);
  const Webserv &operator=(const Webserv &other);

  const int &getServerFD(void) const;
  const unsigned int &getPort(void) const;

  int setNonBlocking(int fd);
  void serverListeningLoop(int serverFD);
  std::vector<pollfd> initializePollFDSWithServerSocket(int serverFD);
  void monitorSocketEvents(std::vector<pollfd> &fds, int serverFD);
  struct pollfd setNewTempFDStruct(int newSocket);
  void processConnectionData(int serverFD, std::vector<pollfd> &fds, size_t &i,
                             std::map<size_t, std::string> &buffers);
  void processClientInput(std::string clientInput, int serverFD, int clientFD,
                          std::string &staticBuffer);

  // std::vector<ServerConfig> config;

  void countAndParseServer(const char *filename);
};

/* ---------------------UTILS FUNCTIONS */
int sendall(int s, const char *buf, int len);

#endif