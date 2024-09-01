/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Webserv.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: demre <demre@student.42malaga.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/31 18:11:45 by demre             #+#    #+#             */
/*   Updated: 2024/09/01 18:51:13 by demre            ###   ########.fr       */
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
  std::string boundary;
  const std::vector<char> fileContent; //CHECK IF USED

  // map < pipe_fd, client_fd >, to keep track of which pipe belongs to which client when a cgi script is writing in a pipe
  std::map< int, int > clientScriptMap;

  // map < pipe_fd, pid >, to keep track of which pipe belongs to which pid when a cgi script is being executed
  std::map< int, pid_t > pidMap;
  std::map< int, pid_t > terminatedPidMap;

  Webserv(void);

public:
  std::vector<pollfd> fds;
  std::vector<ClientInfo> clients;

  Webserv(std::vector<ServerConfig> &serverConfigs);
  ~Webserv(void);

  int setNonBlocking(int fd);

  // Get index of client in std::vector<ClientInfo> clients from corresponding pipe_fd in std::map< pipe_fd, client_fd > clientScriptMap
  size_t findClientIndexOfConnectionFromPipeFD(int pipeFD);

  // Get index of client in std::vector<ClientInfo> clients from fd
  size_t findClientIndexFromFD(int fd);

  void createServers(std::vector<ServerConfig> &serverConfigs);

  void handleNewConnection(size_t &index,
                           const std::vector<ServerConfig> &serverConfigs);

  /* REQUEST */

  void handleClientRequest(size_t &index,
                           const std::vector<ServerConfig> &serverConfigs);
  int recvChunk(int sockfd, std::vector<char> &buffer,
                size_t totalBytesReceived, size_t &i);
  void resolveRequestedPathFromLocations(ClientRequest &req,
                                         const ServerConfig &serverConfig);
  bool isMethodAllowedAtLoc(ClientRequest &req,
                            const ServerConfig &serverConfig);

  /* CGI */

  void executeScript(std::string const &filePath, std::string const &scriptType,
                     std::string const &queryString, int &clientFD);
  void executeScript(std::string &filePath, std::string const &scriptType,
                     ClientInfo &client);

  // Read from pipe and save script output to client.response
  void readAndHandleScriptOutput(size_t &index);

  std::string generateCgiOutputHtmlPage(std::string const &output,
                                        std::string const &URIpath);

  /* CLEAN UP */

  // Check for terminated child processes
  void checkTerminatedProcesses();

  // Close client connection and remove from pollfd and clients array, and remove any pending script pipes for that connection
  void closeConnection(size_t &index);

  // Close pipe and remove from pollfd, clients and clientScriptMap array
  void closePipe(size_t &index);

  void parseClientRequest(ClientRequest &req, long long int maxBodySize,
                          size_t &i, bool &isError);
  void checkBodySize(ClientRequest &req, long long int maxBodySize, size_t &i);

  // Checks if the connection should be closed because of the request
  bool checkCloseConnectionReq(ClientRequest &req);

  // Checks if the connection should be closed because of the reponse
  bool checkCloseConnectionResp(std::vector<char> response);

  void handleClientResponse(size_t &index);

  //SIGNAL
  static void sigInt(int code);
  void handleSigInt(int code);
  static Webserv *instance;

  //COOKIES
  std::map<std::string, Cookies> sessionIdMap;
  std::vector<std::string> sessionID;
  void parseCookies(ClientRequest req);
  std::string getCookieLine(const std::string &clientInput) const;
};
