/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerConfig.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: demre <demre@student.42malaga.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/31 18:12:40 by demre             #+#    #+#             */
/*   Updated: 2024/08/01 14:12:18 by demre            ###   ########.fr       */
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
  std::string serverName;

public:
  ServerConfig(void);
  ~ServerConfig();

  const std::string &getServerName(void) const;
  const unsigned int &getPort(void) const;
  const std::string &getHost(void) const;

  static std::vector<ServerConfig> parseConfig(const char *filename);
  static bool checkConfig(ServerConfig &config);
};

#endif
