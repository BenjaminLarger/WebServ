/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ClientInfo.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: demre <demre@student.42malaga.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/31 20:29:41 by demre             #+#    #+#             */
/*   Updated: 2024/08/10 12:27:13 by demre            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

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

  int socketFD;       // Client socket file descriptor
  int port;           // Server port where client is connected
  size_t serverIndex; // Index of the server in serverConfigs vector

  ClientRequest req;
};
