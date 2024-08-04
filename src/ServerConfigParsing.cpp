/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerConfigParsing.cpp                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: demre <demre@student.42malaga.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/02 14:33:15 by demre             #+#    #+#             */
/*   Updated: 2024/08/04 17:31:06 by demre            ###   ########.fr       */
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
  locations.clear();
  serverRoot.clear();
  serverIndex.clear();
}

std::vector<ServerConfig> ServerConfig::parseConfigs(const char *filename)
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
    std::stringstream ss(line);
    std::string key;

    // std::cout << "line: '" << line << "'" << std::endl;
    if (line.size() == 8 && line.find("server {") != std::string::npos)
    {
      if (insideServerBlock)
        file.close(),
            throw(std::runtime_error("Invalid server block in config file."));

      insideServerBlock = true;
      config.reset();
      tempPorts.clear();
    }
    else if (line.size() && line[0] == '#')
      continue;
    else if (insideServerBlock && line.size() == 1 && line[0] == '}')
      config.endServerBlock(insideServerBlock, serverConfigs, tempPorts, file);
    else if (insideServerBlock && ss >> key)
    {
      std::string valueStr;
      long long valueLong;

      // std::cout << "line: '" << line << "'" << std::endl;
      // std::cout << "key: '" << key << "'" << std::endl;

      validateAndSanitizeServerLine(line, ss, key, file);

      if (key.size() && key[0] == '#')
        continue;
      else if (key == "host")
      {
        ss >> valueStr;
        if (config.host.size() || !valueStr.size()
            || checkStreamForRemainingContent(ss))
          file.close(), throw(std::runtime_error(
                            "Unexpected characters in config file: " + line));

        config.host = valueStr;
      }
      else if (key == "listen")
      {
        ss >> valueLong;
        if (valueLong < 1024 || valueLong > 65535
            || checkStreamForRemainingContent(ss))
          file.close(), throw(std::runtime_error(
                            "Incorrect port in config file: " + line));

        tempPorts.push_back(valueLong);
        if (hasDuplicates(tempPorts))
          file.close(),
              throw(std::runtime_error("Duplicate ports in config file."));
      }
      else if (key == "server_names")
      {
        while (ss >> valueStr)
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
        ss >> valueLong;
        if (config.maxBodySize > 0 || valueLong < 1 || valueLong > INT_MAX
            || checkStreamForRemainingContent(ss))
          file.close(),
              throw(std::runtime_error(
                  "Incorrect client_max_body_size in config file: " + line));

        config.maxBodySize = valueLong;
      }
      else if (key == "location")
      {
        std::string urlPattern;
        ss >> urlPattern >> valueStr;

        if (valueStr != "{")
          file.close(), throw(std::runtime_error(
                            "Invalid location block in config file: " + line));

        config.parseLocation(file, urlPattern);
      }
      else if (key == "error_page")
      {
      }
      else if (key == "root")
      {
        ss >> valueStr;
        if (config.serverRoot.size() || !valueStr.size()
            || checkStreamForRemainingContent(ss))
          file.close(),
              throw(std::runtime_error(
                  "Unexpected characters in location block: " + line));
        config.serverRoot = valueStr;
      }
      else if (key == "index")
      {
        ss >> valueStr;
        if (config.serverIndex.size() || !valueStr.size()
            || checkStreamForRemainingContent(ss))
          file.close(),
              throw(std::runtime_error(
                  "Unexpected characters in location block: " + line));
        config.serverIndex = valueStr;
      }
      else if (key.size() && !isAllWhitespace(key))
        file.close(),
            throw(std::runtime_error("Invalid data in config file: " + key));
    }
    else if (line.size() && !isAllWhitespace(line))
      file.close(),
          throw(std::runtime_error("Invalid data in config file: " + line));
  }

  if (insideServerBlock)
    file.close(),
        throw(std::runtime_error("Invalid server block in config file."));

  file.close();
  return (serverConfigs);
}

void ServerConfig::validateAndSanitizeServerLine(std::string &line,
                                                 std::stringstream &ss,
                                                 std::string &key,
                                                 std::ifstream &file)
{
  // Check line ends correctly
  if (line.size() && line[line.size() - 1] != ';' && key.size() && key[0] != '#'
      && key != "}" && line[line.size() - 1] != '{')
    file.close(),
        throw(std::runtime_error("Invalid end of line in config file." + line));

  // Remove trailing ";" if present and trim ws
  if (line.size() && line[line.size() - 1] == ';')
    line.erase(line.size() - 1);
  trimTrailingWS(line);

  // Reload updated line
  ss.str("");
  ss << line;
  ss >> key;
}

bool ServerConfig::checkConfig(std::vector<int> &tempPorts)
{
  // Add default host if needed
  if (this->getHost().size() == 0)
  {
    std::string value = "127.0.0.1";
    this->host = value.c_str();
  }

  // Check host is valid
  for (std::string::const_iterator it = this->host.begin();
       it != this->host.end(); ++it)
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
                                  std::vector<int> &tempPorts,
                                  std::ifstream &file)
{
  // std::cout << "tempPorts.size(): " << tempPorts.size() << std::endl;
  if (checkConfig(tempPorts) && insideServerBlock)
  {
    for (size_t i = 0; i < tempPorts.size(); i++)
    { // Create a new serverConfig for each port of each server
      this->addPort(tempPorts[i]);
      serverConfigs.push_back(*this);
    }
  }
  else
  {
    file.close();
    throw(std::runtime_error("Invalid server block in config file."));
  }
  insideServerBlock = false;
}
