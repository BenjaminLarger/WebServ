/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerConfigParsing.cpp                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: demre <demre@student.42malaga.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/02 14:33:15 by demre             #+#    #+#             */
/*   Updated: 2024/08/02 17:49:09 by demre            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ServerConfig.hpp"
#include "utils.hpp"

void ServerConfig::reset()
{
  host.clear();
  port = -1;
  maxBodySize = -1;
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
    trimTrailingWS(line);
    std::stringstream iss(line);
    std::string key, valueStr;
    int valueInt;

    if (line.size() == 8 && line.find("server {") != std::string::npos)
    {
      insideServerBlock = true;
      config.reset();
      tempPorts.clear();
    }
    else if (line[0] == '#')
      continue;
    else if (line.size() == 1 && line.find("}") != std::string::npos)
      endServerBlock(insideServerBlock, serverConfigs, config, tempPorts, file);
    else if (insideServerBlock && iss >> key)
    {
      // Check line ends with ";" and remove it
      // std::cout << "line: '" << line << "'" << std::endl;
      if (line[line.size() - 1] != ';' && key[0] != '#')
        file.close(),
            throw(std::runtime_error("Invalid end of line in config file."));
      line.erase(line.size() - 1);
      trimTrailingWS(line);

      iss.str("");
      iss << line;
      iss >> key;

      if (key[0] == '#')
        continue;
      else if (key == "host")
      {
        iss >> valueStr;
        config.host = valueStr;

        if (checkStreamForRemainingContent(iss))
          file.close(), throw(std::runtime_error(
                            "Unexpected characters in config file: " + line));
      }
      else if (key == "listen")
      {
        iss >> valueInt;
        int port = valueInt;
        tempPorts.push_back(port);

        if (checkStreamForRemainingContent(iss))
          file.close(), throw(std::runtime_error(
                            "Unexpected characters in config file: " + line));
        if (hasDuplicates(tempPorts))
          file.close(),
              throw(std::runtime_error("Duplicate ports in config file."));
      }
      else if (key == "server_names")
      {
        while (iss >> valueStr)
        {
          if (!isAllWhitespace(valueStr))
            config.addServerName(valueStr);
        }
        if (hasDuplicates(config.serverNames))
          file.close(), throw(std::runtime_error(
                            "Duplicate server names in config file."));
      }
      else if (key == "client_max_body_size")
      {
        iss >> valueInt;
        config.maxBodySize = valueInt;

        if (checkStreamForRemainingContent(iss))
          file.close(), throw(std::runtime_error(
                            "Unexpected characters in config file: " + line));
      }
      else if (key.size() > 0 && !isAllWhitespace(key))
        file.close(),
            throw(std::runtime_error("Invalid data in config file: " + key));
    }
    else if (line.size() > 0 && !isAllWhitespace(line))
      file.close(),
          throw(std::runtime_error("Invalid data in config file: " + line));
  }
  if (insideServerBlock)
    file.close(),
        throw(std::runtime_error("Invalid server block in config file."));

  file.close();
  return (serverConfigs);
}

bool ServerConfig::checkConfig(ServerConfig &config,
                               std::vector<int> &tempPorts)
{
  // Add default host if needed
  if (config.getHost().size() == 0)
  {
    std::string value = "127.0.0.1";
    config.host = value.c_str();
  }

  // Check host is valid
  for (std::string::const_iterator it = config.host.begin();
       it != config.host.end(); ++it)
  {
    if (!isdigit(*it) && *it != '.')
      return (false);
  }

  // Check port present
  if (tempPorts.size() == 0)
    return (false);
  return (true);
}

void ServerConfig::endServerBlock(bool &insideServerBlock,
                                  std::vector<ServerConfig> &serverConfigs,
                                  ServerConfig &config,
                                  std::vector<int> &tempPorts,
                                  std::ifstream &file)
{
  insideServerBlock = false;
  // std::cout << "tempPorts.size(): " << tempPorts.size() << std::endl;
  if (checkConfig(config, tempPorts))
  {
    for (size_t i = 0; i < tempPorts.size(); i++)
    { // Create a new serverConfig for each port of each server
      config.addPort(tempPorts[i]);
      serverConfigs.push_back(config);
    }
  }
  else
  {
    file.close();
    throw(std::runtime_error("Invalid server block in config file."));
  }
}
