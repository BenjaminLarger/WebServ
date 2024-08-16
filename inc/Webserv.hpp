/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Webserv.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: demre <demre@student.42malaga.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/31 18:11:45 by demre             #+#    #+#             */
/*   Updated: 2024/08/16 13:58:01 by demre            ###   ########.fr       */
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

struct Cookies
{
};
class Webserv
{
private:
  std::vector<pollfd> fds;

  // map < pipe_fd, client_fd >, to keep track of which pipe belongs to which client when a cgi script is writing in a pipe
  std::map< int, int > clientScriptMap;

  // map < pipe_fd, pid >, to keep track of which pipe belongs to which pid when a cgi script is being executed
  std::map< int, pid_t > pidMap;

  int contentLength;

  Webserv(void);

public:
  std::vector<ClientInfo> clients;

  Webserv(std::vector<ServerConfig> &serverConfigs);
  ~Webserv(void);

  int setNonBlocking(int fd);

  ssize_t recvAll(int sockfd, std::vector<char> &buffer);

  void createServers(std::vector<ServerConfig> &serverConfigs);

  void handleNewConnection(size_t &index,
                           const std::vector<ServerConfig> &serverConfigs);
  bool isMethodAllowedAtLoc(ClientRequest &req,
                            const ServerConfig &serverConfig);
  void handleClientRequest(size_t &index,
                           const std::vector<ServerConfig> &serverConfigs);

  // Read from pipe and save script output to client.response
  void readAndHandleScriptOutput(size_t &index);

  // Get index of client in std::vector<ClientInfo> clients from corresponding pipe_fd in std::map< pipe_fd, client_fd > clientScriptMap
  size_t findClientIndexFromPipeFD(int pipeFD);

  // Get index of client in std::vector<ClientInfo> clients from clientFD
  size_t findClientIndexFromClientFD(int clientFD);

  std::string generateCgiOutputHtmlPage(const std::string &output);

  // Close client connection and remove from pollfd and clients array, and remove any pending script pipes for that connection
  void closeConnection(size_t &index);

  // Close pipe and remove from pollfd, clients and clientScriptMap array
  void closePipe(size_t &index);

  void parseClientRequest(ClientRequest &req);
  void resolveRequestedPathFromLocations(ClientRequest &req,
                                         const ServerConfig &serverConfig);

  void executeScript(std::string const &filePath, std::string const &scriptType,
                     int &clientFD);

  void handleClientResponse(size_t &index);

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
  std::map<std::string, Cookies> sessionIdMap;
  std::vector<std::string> sessionID;
  void parseCookies(ClientRequest req);
  std::string getCookieLine(const std::string &clientInput) const;
};
