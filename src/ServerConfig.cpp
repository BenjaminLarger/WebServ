/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerConfig.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: demre <demre@student.42malaga.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/31 18:15:10 by demre             #+#    #+#             */
/*   Updated: 2024/08/02 14:24:17 by demre            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* HTML5 references

There are three possibilities for enctype:
- application/x-www-form-urlencoded
- multipart/form-data (spec points to RFC7578)
- text/plain. This is "not reliably interpretable by computer", so it should never be used in production, and we will not look further into it. */

#include "ServerConfig.hpp"
#include "utils.hpp"

ServerConfig::ServerConfig(void) {}

ServerConfig::~ServerConfig(void) {}

/* --------------GETTER */

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

const std::string &ServerConfig::getServerName(int index) const
{
  return (this->serverNames[index]);
}

const int &ServerConfig::getPort() const { return (this->port); }

const std::string &ServerConfig::getHost(void) const { return (this->host); }

/* --------------SETTER */

void ServerConfig::addServerName(std::string newServerName)
{
  serverNames.push_back(newServerName);
}

void ServerConfig::addPort(int newPort) { this->port = newPort; }

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
  std::vector<int> tempPorts;
  bool insideServerBlock = false;

  while (std::getline(file, line))
  {
    std::istringstream iss(line);
    std::string key, value;

    if (line.size() == 8 && line.find("server {") != std::string::npos)
    {
      insideServerBlock = true;
      config.clear();
      tempPorts.clear();
    }
    else if (insideServerBlock && iss >> key >> value)
    {
      if (line[line.size() - 1] != ';') // Check line ends with ";"
        throw(std::runtime_error("Invalid end of line in config file."));

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
        int port = std::atoi(value.c_str());
        tempPorts.push_back(port);
        // config.addPort(port);
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
      else if (key.size() > 0 && !isAllWhitespace(key))
        throw(std::runtime_error("Invalid data in config file: " + key));
    }
    else if (line.size() == 1 && line.find("}") != std::string::npos)
    {
      endServerBlock(insideServerBlock, serverConfigs, config, tempPorts);
    }
    else if (line.size() > 0 && !isAllWhitespace(line))
      throw(std::runtime_error("Invalid data in config file: " + line));
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
  // if (config.getPort() == -1)
  //   return (false);
  return (true);
}

void ServerConfig::endServerBlock(bool &insideServerBlock,
                                  std::vector<ServerConfig> &serverConfigs,
                                  ServerConfig &config,
                                  std::vector<int> &tempPorts)
{
  insideServerBlock = false;

  if (checkConfig(config) && tempPorts.size() > 0)
  {
    for (size_t i = 0; i < tempPorts.size(); i++)
    { // Create a new serverConfig for each port for each server
      config.addPort(tempPorts[i]);
      serverConfigs.push_back(config);
    }
  }
  else
  {
    // handle wrong server config
    std::cerr << "checkConfig is false" << std::endl;
  }
}

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
