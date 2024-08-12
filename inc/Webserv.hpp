/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Webserv.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: demre <demre@student.42malaga.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/31 18:11:45 by demre             #+#    #+#             */
/*   Updated: 2024/08/12 19:51:17 by demre            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "ClientInfo.hpp"
#include "HttpExceptions.hpp"
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

  // map < pipe_fd, client_fd >, to keep track of which pipe belongs to which client when a cgi script is writing in a pipe
  std::map< int, int > clientScriptMap;

  int contentLength;

  Webserv(void);

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
  bool isMethodAllowedAtLoc(ClientRequest &req,
                            const ServerConfig &serverConfig);
  void handleClientRequest(size_t index,
                           const std::vector<ServerConfig> &serverConfigs);

  void closeConnection(size_t index);

  void parseClientRequest(ClientRequest &req);
  void resolveRequestedPathFromLocations(ClientRequest &req,
                                         const ServerConfig &serverConfig);

  void executeScript(std::string const &filePath, std::string const &scriptType,
                     int &clientFD);

  void handleClientResponse(size_t index);

  //SIGNAL
  static void sigInt(int code);
  void handleSigInt(int code);
  static Webserv *instance;

  //MANAGE CLIENT CAPABILITY
  void withdrawReadCapability(size_t clientIndex, std::string &buffer);
  void withdrawWriteCapability(size_t clientIndex, std::string &buffer);
  void restoreReadCapability(size_t clientIndex, std::string &buffer);
  void restoreWriteCapability(size_t clientIndex, std::string &buffer);

  //COOKIES
  void parseCookies(ClientRequest req);
  std::string getCookieLine(const std::string &clientInput) const;
};
