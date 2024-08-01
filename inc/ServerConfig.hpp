/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerConfig.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: demre <demre@student.42malaga.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/31 18:12:40 by demre             #+#    #+#             */
/*   Updated: 2024/08/01 16:04:57 by demre            ###   ########.fr       */
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
  unsigned int port;
  std::vector<std::string> serverNames;

public:
  ServerConfig(void);
  ~ServerConfig();

  std::string getServerNames(void) const;
  const unsigned int &getPort(void) const;
  const std::string &getHost(void) const;

  static std::vector<ServerConfig> parseConfig(const char *filename);

  static void parseServerNames(const std::string &value,
                               std::vector<std::string> &serverNames);

  static bool checkConfig(ServerConfig &config);
};

#endif
