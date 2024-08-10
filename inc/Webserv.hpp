/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Webserv.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blarger <blarger@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/31 18:11:45 by demre             #+#    #+#             */
/*   Updated: 2024/08/10 18:16:31 by blarger          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "ClientInfo.hpp"
#include "ServerConfig.hpp"
#include "dependencies.hpp"

// ************************************************************************** //
//                               Class //
// ************************************************************************** //

class Webserv
{
private:
  std::vector<pollfd> fds;
  std::vector<ClientInfo> clients;
  int contentLength;

  Webserv(void);
  const Webserv &operator=(const Webserv &other);

public:
  Webserv(std::vector<ServerConfig> &serverConfigs);
  ~Webserv(void);

  int setNonBlocking(int fd);

  // void processClientInput(std::string clientInput, int serverFD, int clientFD,
  //                         std::string &staticBuffer);

  ssize_t recvAll(int sockfd, std::vector<char> &buffer);

  void createServers(std::vector<ServerConfig> &serverConfigs);

  void handleNewConnection(size_t index,
                           const std::vector<ServerConfig> &serverConfigs);
  bool isMethodAllowedAtLoc(std::string method, ClientRequest &req,
                            const ServerConfig &serverConfig);
  void handleClientRequest(size_t index,
                           const std::vector<ServerConfig> &serverConfigs);
  //void manageRequestLocation(size_t serverIndex, const std::vector<ServerConfig> &serverConfigs);
  void closeConnection(size_t index);

  void parseClientRequest(ClientRequest &req, std::string &clientStr);
  void handleLocations(ClientRequest &req, const ServerConfig &serverConfig);

  //SIGNAL
  static void sigInt(int code);
  void handleSigInt(int code);
  static Webserv *instance;

  //MANAGE CLIENT CAPABILITY
  void withdrawReadCapability(size_t clientIndex, std::string &buffer);
  void withdrawWriteCapability(size_t clientIndex, std::string &buffer);
  void restoreReadCapability(size_t clientIndex, std::string &buffer);
  void restoreWriteCapability(size_t clientIndex, std::string &buffer);
};
