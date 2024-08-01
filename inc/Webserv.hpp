/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Webserv.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blarger <blarger@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/31 18:11:45 by demre             #+#    #+#             */
/*   Updated: 2024/08/01 13:41:30 by blarger          ###   ########.fr       */
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
  Webserv(std::vector<ServerConfig> &serverConfigs);
  ~Webserv(void);

  int setNonBlocking(int fd);

  // void processClientInput(std::string clientInput, int serverFD, int clientFD,
  //                         std::string &staticBuffer);

  void createServers(std::vector<ServerConfig> &serverConfigs);

  void handleNewConnection(size_t index,
                           const std::vector<ServerConfig> &serverConfigs);
  void handleClientRequest(size_t index,
                           const std::vector<ServerConfig> &serverConfigs);
  void closeConnection(size_t index);

  //SIGNAL
  static void	sigInt(int code);
  void handleSigInt(int code);
  static Webserv* instance;

  //MANAGE CLIENT CAPABILITY
  void	withdrawReadCapability(size_t &clientIndex, std::string &buffer);
  void	withdrawWriteCapability(size_t &clientIndex, std::string &buffer);
  void	restoreReadCapability(size_t &clientIndex, std::string &buffer);
  void	restoreWriteCapability(size_t &clientIndex, std::string &buffer);
};



/* ---------------------UTILS FUNCTIONS */
int sendall(int s, const char *buf, int len);

#endif