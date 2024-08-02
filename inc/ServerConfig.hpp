/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerConfig.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: demre <demre@student.42malaga.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/31 18:12:40 by demre             #+#    #+#             */
/*   Updated: 2024/08/02 15:51:23 by demre            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#ifndef SERVER_CONFIG_HPP
#define SERVER_CONFIG_HPP

#include "Dependencies.hpp"

// ************************************************************************** //
//                               Class //
// ************************************************************************** //

class ServerConfig
{
private:
  std::string host;
  int port;
  std::vector<std::string> serverNames;

public:
  ServerConfig(void);
  ~ServerConfig();

  // Returns a list of all the server names as one string (to display in terminal)
  std::string getServerNames(void) const;

  // Returns the server name at the given index in the serverNames vector
  const std::string &getServerName(int index) const;
  // Returns the port at the given index in the ports vector
  const int &getPort(void) const;

  const std::string &getHost(void) const;

  // Add a server name to the serverNames vector
  void addServerName(std::string newServerName);
  // Add a port to the ports vector
  void addPort(int newPort);

  // Clears the attributes from the current ServerConfig object
  void clear(void);

  // Parses server configs to a vector of ServerConfig from a config file
  static std::vector<ServerConfig> parseConfig(const char *filename);

  // Handles ServerConfig when reaching the end of a server block in the configfile
  static void endServerBlock(bool &insideServerBlock,
                             std::vector<ServerConfig> &serverConfigs,
                             ServerConfig &config, std::vector<int> &tempPorts,
                             std::ifstream &file);

  // Checks that the ServerConfig has valid and complete data
  static bool checkConfig(ServerConfig &config, std::vector<int> &tempPorts);
};

#endif
