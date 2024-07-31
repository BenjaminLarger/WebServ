/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Webserv.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: demre <demre@student.42malaga.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/31 18:11:45 by demre             #+#    #+#             */
/*   Updated: 2024/07/31 21:30:11 by demre            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#ifndef WEBSERV_HPP
#define WEBSERV_HPP

// #include "ConfigurationFile.hpp"
#include "ClientInfo.hpp"
#include "Dependencies.hpp"
#include "ServerConfig.hpp"

// ************************************************************************** //
//                               Class //
// ************************************************************************** //

class Webserv
{
private:
  // std::vector<ServerConfig> serverConfigs;
  // ServerConfig config;
  // unsigned int port;
  // int serverFD;
  std::vector<pollfd> fds;
  std::vector<ClientInfo> clients;

  // std::vector<int> serverFds;

  // int optval;
  // unsigned int numberOfServers;

  const Webserv &operator=(const Webserv &other);

public:
  Webserv(void);
  Webserv(std::vector<ServerConfig> serverConfigs);
  ~Webserv(void);

  int setNonBlocking(int fd);

  // void processClientInput(std::string clientInput, int serverFD, int clientFD,
  //                         std::string &staticBuffer);

  void handleNewConnection(size_t index,
                           const std::vector<ServerConfig> &serverConfigs);
  void handleClientData(size_t index,
                        const std::vector<ServerConfig> &serverConfigs);
  void closeConnection(size_t index);
};

/* ---------------------UTILS FUNCTIONS */
int sendall(int s, const char *buf, int len);

#endif