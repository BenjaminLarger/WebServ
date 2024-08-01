/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerConfig.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: demre <demre@student.42malaga.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/31 18:15:10 by demre             #+#    #+#             */
/*   Updated: 2024/08/01 17:29:39 by demre            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ServerConfig.hpp"

ServerConfig::ServerConfig(void) {}

ServerConfig::~ServerConfig(void) {}

/* --------------GETTER */

const unsigned int &ServerConfig::getPort(void) const { return (this->port); }

std::string ServerConfig::getServerNames(void) const
{
  std::string serverNamesStr;
  for (size_t i = 0; i < this->serverNames.size(); i++)
  {
    if (i > 0)
    {
      serverNamesStr += ' ';
    }
    serverNamesStr += this->serverNames[i];
  }
  return (serverNamesStr);
}

const std::string &ServerConfig::getHost(void) const { return (this->host); }

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
  std::string serverNamesAccumulator;

  while (std::getline(file, line))
  {
    std::istringstream iss(line);
    std::string key, value;

    if (line.find("server {") != std::string::npos)
    {
      inside_server_block = true;
      serverNamesAccumulator.clear();
    }
    else if (inside_server_block && iss >> key)
    {
      std::getline(iss, value);

      if (key == "}" && value.empty())
      {
        inside_server_block = false;

        parseServerNames(serverNamesAccumulator, config.serverNames);

        if (checkConfig(config))
          serverConfigs.push_back(config);
        else
        {
          // handle wrong server config
          std::cerr << "checkConfig is false" << std::endl;
        }
        continue;
      }

      if (!value.empty() && value[0] == ' ')
        value.erase(0, 1); // Remove leading space
      if (!value.empty() && value[value.size() - 1] == ';')
        value.erase(value.size() - 1); // Remove trailing semicolon

      if (key == "listen")
        config.port = std::atoi(value.c_str());
      else if (key == "server_names")
      {
        if (!serverNamesAccumulator.empty())
          serverNamesAccumulator += ' ';
        serverNamesAccumulator += value;
      }
      else if (key == "host")
        config.host = value;
    }
    // else if (line.find("}") != std::string::npos)
    // {
    //   inside_server_block = false;

    //   parseServerNames(serverNamesAccumulator, config.serverNames);

    //   std::cout << ".getHost() " << config.getHost() << " .getPort() "
    //             << config.getPort() << std::endl;

    //   if (checkConfig(config))
    //     serverConfigs.push_back(config);
    //   else
    //   {
    //     // handle wrong server config
    //     std::cerr << "checkConfig is false" << std::endl;
    //   }
    // }
  }

  file.close();
  return (serverConfigs);
}

bool ServerConfig::checkConfig(ServerConfig &config)
{
  // check host and port present
  if (config.getHost().size() == 0)
  {
    std::string value = "127.0.0.1";
    config.host = value.c_str();
  }
  // if (!config.getPort())
  //   return (false);
  return (true);
}

void ServerConfig::parseServerNames(const std::string &value,
                                    std::vector<std::string> &serverNames)
{
  std::istringstream iss(value);
  std::string serverName;
  while (iss >> serverName)
  {
    // Remove any trailing semicolon or space
    if (!serverName.empty() && serverName[serverName.size() - 1] == ';')
      serverName.erase(serverName.size() - 1);
    serverNames.push_back(serverName);
  }
}
