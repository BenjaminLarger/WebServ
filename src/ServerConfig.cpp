/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerConfig.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: demre <demre@student.42malaga.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/31 18:15:10 by demre             #+#    #+#             */
/*   Updated: 2024/08/01 20:03:14 by demre            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ServerConfig.hpp"
#include "utils.hpp"

ServerConfig::ServerConfig(void) {}

ServerConfig::~ServerConfig(void) {}

/* --------------GETTER */

const int &ServerConfig::getPort(void) const { return (this->port); }

std::string ServerConfig::getServerNames(void) const
{
  std::cout << ".size(" << serverNames.size() << ") ";

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

/* --------------SETTER */

void ServerConfig::addServerName(std::string serverName)
{
  serverNames.push_back(serverName);
}

/* --------------MEMBER FUNCTIONS */

void ServerConfig::clear()
{
  host.clear();
  port = -1;
  serverNames.clear();
}

std::vector<ServerConfig> ServerConfig::parseConfig(const char *filename)
{
  std::ifstream file(filename);
  if (!file.is_open())
    throw(std::runtime_error("Could not open the configuration file."));

  std::vector<ServerConfig> serverConfigs;
  std::string line;
  ServerConfig config;
  bool insideServerBlock = false;

  while (std::getline(file, line))
  {
    std::istringstream iss(line);
    std::string key, value;

    if (line.find("server {") != std::string::npos)
    {
      insideServerBlock = true;
      config.clear();
    }
    else if (insideServerBlock && iss >> key >> value)
    {
      if (!value.empty() && value[0] == ' ')
        value.erase(0, 1); // Remove leading space
      if (!value.empty() && value[value.size() - 1] == ';')
        value.erase(value.size() - 1); // Remove trailing semicolon

      if (key == "host")
      {
        config.host = value;
        checkRemainingChar(iss);
      }
      else if (key == "listen")
      {
        config.port = std::atoi(value.c_str());
        checkRemainingChar(iss);
      }
      else if (key == "server_names")
      {
        config.addServerName(value);

        while (iss >> value)
        {
          if (!value.empty() && value[value.size() - 1] == ';')
            value.erase(value.size() - 1);
          if (!isAllWhitespace(value))
            config.addServerName(value);
        }
      }
    }
    else if (line.find("}") != std::string::npos)
    {
      endServerBlock(insideServerBlock, serverConfigs, config);
    }
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
  if (config.getPort() == -1)
    return (false);
  return (true);
}

void ServerConfig::endServerBlock(bool &insideServerBlock,
                                  std::vector<ServerConfig> &serverConfigs,
                                  ServerConfig &config)
{
  insideServerBlock = false;

  if (checkConfig(config))
    serverConfigs.push_back(config);
  else
  {
    // handle wrong server config
    std::cerr << "checkConfig is false" << std::endl;
  }
}

// Check if there's anything left in iss after extracting value
void ServerConfig::checkRemainingChar(std::istringstream &iss)
{
  std::string remaining;
  iss >> remaining;
  if (!remaining.empty() && remaining[remaining.size() - 1] == ';')
    remaining.erase(remaining.size() - 1); // Remove trailing semicolon
  // std::cout << "remaining: '" << remaining << "'" << std::endl;
  if (!remaining.empty())
    throw(std::runtime_error("Unexpected characters in config file: "
                             + remaining));
}
