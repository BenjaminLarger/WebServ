/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigurationFile.hpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: demre <demre@student.42malaga.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/03 11:23:15 by blarger           #+#    #+#             */
/*   Updated: 2024/07/09 18:16:53 by demre            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#ifndef CONFIGURATION_FILE_HPP
#define CONFIGURATION_FILE_HPP

#include "Dependencies.hpp"

// ************************************************************************** //
//                               Class                                        //
// ************************************************************************** //

class LocationConfig
{
private:
public:
  std::string root;
  std::string index;
  std::vector<std::string> allowedMethods;
  bool autoindexOn;
};

class ServerConfig
{

private:
  unsigned int port;
  std::string serverName;
  std::map<int, std::string> errorPages;
  std::map<std::string, LocationConfig> locations;
  std::string host;
  unsigned int maxBodySize;

public:
  ServerConfig(const char *filename);
  ServerConfig();
  ServerConfig parsConfigFile(char *filename);

  const int &getServerFD(void) const;
  const unsigned int &getPort(void) const;

  std::map<int, std::string> findErrorPage(std::istringstream &isLine);
  void findLocation(std::string &line, ServerConfig &config,
                    std::ifstream &file, std::string currentLocation);
};

/* ---------------------UTILS FUNCTIONS */
// Configuration file parsing

#endif