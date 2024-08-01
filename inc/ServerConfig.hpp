/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerConfig.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: demre <demre@student.42malaga.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/31 18:12:40 by demre             #+#    #+#             */
/*   Updated: 2024/08/01 19:48:31 by demre            ###   ########.fr       */
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

  std::string getServerNames(void) const;
  const int &getPort(void) const;
  const std::string &getHost(void) const;

  void addServerName(std::string serverName);

  void clear(void);

  static std::vector<ServerConfig> parseConfig(const char *filename);

  static void checkRemainingChar(std::istringstream &iss);

  static void endServerBlock(bool &insideServerBlock,
                             std::vector<ServerConfig> &serverConfigs,
                             ServerConfig &config);

  static bool checkConfig(ServerConfig &config);
};

#endif
