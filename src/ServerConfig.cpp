/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerConfig.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: demre <demre@student.42malaga.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/31 18:15:10 by demre             #+#    #+#             */
/*   Updated: 2024/07/31 18:32:47 by demre            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ServerConfig.hpp"

ServerConfig::ServerConfig(void) {}

ServerConfig::~ServerConfig(void) {}

/* --------------GETTER */

const unsigned int &ServerConfig::getPort(void) const { return (this->port); }

const std::string &ServerConfig::getServerName(void) const
{
  return (this->serverName);
}

/* --------------MEMBER FUNCTIONS */

std::vector<ServerConfig> ServerConfig::parseConfig(const char *filename)
{
  std::ifstream file(filename);
  if (!file.is_open())
    throw(std::runtime_error("Could not open the configuration file."));

  std::vector<ServerConfig> serverConfigs;
  std::string line;
  ServerConfig config;
  bool inside_server_block = false;

  while (std::getline(file, line))
  {
    std::istringstream iss(line);
    std::string key, value;

    if (line.find("server {") != std::string::npos)
    {
      inside_server_block = true;
    }
    else if (inside_server_block && iss >> key >> value)
    {
      if (!value.empty() && value[value.size() - 1] == ';')
        value.erase(value.size() - 1);
      if (key == "listen")
        config.port = std::atoi(value.c_str());
      if (key == "server_name")
        config.serverName = value.c_str();
    }
    else if (line.find("}") != std::string::npos)
    {
      inside_server_block = false;
      serverConfigs.push_back(config);
    }
  }

  file.close();
  return (serverConfigs);
}
