/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ClientInfo.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: demre <demre@student.42malaga.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/31 20:29:41 by demre             #+#    #+#             */
/*   Updated: 2024/08/08 16:02:46 by demre            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#ifndef CLIENT_INFO_HPP
#define CLIENT_INFO_HPP

#include "dependencies.hpp"

struct ClientRequest
{
  std::string buffer; // Appended whole request sent by client

  // HTTP request line: GET / HTTP/1.1
  std::string method;
  std::string pathToRessource;
  std::string HTTPversion;

  // Every other header field
  std::map< std::string, std::string > fields;
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

#endif