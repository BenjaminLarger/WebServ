/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ClientInfo.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: demre <demre@student.42malaga.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/31 20:29:41 by demre             #+#    #+#             */
/*   Updated: 2024/08/16 10:17:21 by demre            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "ServerConfig.hpp"
#include "dependencies.hpp"

struct ClientRequest
{
  std::string buffer; // Appended whole request sent by client

  // HTTP request line
  std::string method;      // GET
  std::string URI;         // /new/new.png           /new/test
  std::string HTTPversion; // HTTP/1.1

  // Every other header field
  std::map< std::string, std::string > fields;

  std::string pathFolder;         // /new                   /new
  std::string pathOnServer;       // ./var/www/new/new.png  ./var/www/new/test/
  std::string pathFolderOnServer; //./var/www/new/          ./var/www/new/
};

class ClientInfo
{
private:
public:
  ClientInfo(void);
  ~ClientInfo(void);

  int socketFD; // Client socket file descriptor
  int port;     // Server port where client is connected
  ServerConfig
      client_serverConfig; // ServerConfig of the server where client is connected

  // Client request data
  ClientRequest req;

  // Client response data

  // Temporary string to build response to be sent to client
  std::string responseBuffer;
  // Response to be sent to client
  std::string response;
  // Total bytes to send to client (= response.size())
  int totalToSend;
  // Total bytes sent to client
  int totalBytesSent;
};
