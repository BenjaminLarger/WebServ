/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerConfigParsing.cpp                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blarger <blarger@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/02 14:33:15 by demre             #+#    #+#             */
/*   Updated: 2024/08/29 14:53:43 by blarger          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ServerConfig.hpp"
#include "core.hpp"

void ServerConfig::reset()
{
  host.clear();
  port = -1;
  maxBodySize = -1;
  serverNames.clear();
  locations.clear();
  serverRoot.clear();
  serverIndex.clear();
  errorPages.clear();
}

bool ServerConfig::findSameHostPort(std::vector<ServerConfig> serverConfigs)
{
  for (std::vector<ServerConfig>::iterator it = serverConfigs.begin();
       it != serverConfigs.end(); it++)
  {
    if (it->getHost() == this->getHost() && it->getPort() == this->getPort())
      return (true);
  }
  return (false);
}

std::vector<ServerConfig> ServerConfig::parseConfigs(const char *filename)
{
  std::ifstream file(filename);
  if (!file.is_open())
    throw(std::runtime_error("Could not open the configuration file."));

  std::string line;
  std::vector<ServerConfig> serverConfigs;
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
        if (config.host.size() || valueStr.empty()
            || streamHasRemainingContent(ss))
          file.close(), throw(std::runtime_error(
                            "Unexpected characters in config file: " + line));
        config.host = valueStr;
      }
      else if (key == "listen")
      {
        ss >> valueLong;
        if (valueLong < 1024 || valueLong > 65535
            || streamHasRemainingContent(ss))
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
				//valueLong *= (1024 * 2024); //Value in MB
        if (config.maxBodySize > 0 || valueLong < 1 || valueLong > INT_MAX
            || streamHasRemainingContent(ss))
          file.close(),
              throw(std::runtime_error(
                  "Incorrect client_max_body_size in config file: " + line));

        config.maxBodySize = valueLong;
				std::cout << GREEN << "maxBodySize = " << config.maxBodySize << RESET << std::endl;
      }
      else if (key == "location")
      {
        std::string urlPattern;
        ss >> urlPattern >> valueStr;

        if (valueStr != "{")
          file.close(), throw(std::runtime_error(
                            "Invalid location block in config file: " + line));

        config.parseLocationBlock(file, urlPattern);
      }
      else if (key == "error_page")
      {
        ss >> valueLong >> valueStr;
        if ((!(valueLong >= 400 && valueLong <= 429) && valueLong != 431
             && valueLong != 451 && !(valueLong >= 500 && valueLong <= 508)
             && valueLong != 510 && valueLong != 511)
            || valueStr.empty() || streamHasRemainingContent(ss))
          file.close(), throw(std::runtime_error(
                            "Incorrect error_page in config file: " + line));

        if (hasDuplicates(config.errorPages, static_cast<int>(valueLong)))
          file.close(), throw(std::runtime_error(
                            "Duplicate error_page in config file: " + line));

        config.errorPages[valueLong] = valueStr;
      }
      else if (key == "root")
      {
        ss >> valueStr;
        if (config.serverRoot.size() || valueStr.empty()
            || streamHasRemainingContent(ss))
          file.close(),
              throw(std::runtime_error(
                  "Unexpected characters in location block: " + line));

        config.serverRoot = valueStr;
      }
      else if (key == "index")
      {
        ss >> valueStr;
        if (config.serverIndex.size() || valueStr.empty()
            || streamHasRemainingContent(ss))
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
  if (this->getHost().empty())
  {
    std::string value = "127.0.0.1";
    this->host = value.c_str();
  }
  // else check host is valid
  else
  {
    for (std::string::const_iterator it = this->host.begin();
         it != this->host.end(); ++it)
    {
      if (!isdigit(*it) && *it != '.')
        return (false);
    }
  }

  // Check port present
  if (tempPorts.empty())
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
    // Add default values
    //if (!this->findSameHostPort(serverConfigs))
    //{
    //if (this->getHost().empty())
    //  this->addServerName("127.0.0.1");
    //else
    this->addServerName(this->getHost());
    //}
    // Set default maxBodySize
    if (this->maxBodySize == -1)
      this->maxBodySize = 1000000;

    // Set default index file
    // if (this->serverIndex.empty())
    //   this->serverIndex = "index.html";

    resolveServerPathForLocations();
    resolveServerPathForErrorPages();

    // Create a new serverConfig for each port of each server
    for (size_t i = 0; i < tempPorts.size(); i++)
    {
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

void ServerConfig::resolveServerPathForLocations()
{
  // Set serverPath for top level
  if (serverRoot.size())
    serverPath = "." + formatPath(serverRoot);
  else if (serverRoot.empty())
  {
    for (std::map<std::string, LocationConfig>::iterator it = locations.begin();
         it != locations.end(); it++)
    {
      if (it->first == "/")
      {
        // Append URI to root
        if (it->second.root.size())
        {
          serverRoot = it->second.root;
          serverPath
              = "." + formatPath(it->second.root) + formatPath(it->first);
        }
        // Replace URI with alias
        else if (it->second.alias.size())
          serverPath = "." + formatPath(it->second.alias);
      }
      else
      {
        serverRoot = "";
        serverPath = "";
      }
    }
  }

  // Set serverPath for each location block
  for (std::map<std::string, LocationConfig>::iterator it = locations.begin();
       it != locations.end(); it++)
  {
    // Append URI to root
    if (it->second.root.size())
      it->second.serverPath
          = "." + formatPath(it->second.root) + formatPath(it->first);
    // Replace URI with alias
    else if (it->second.alias.size())
      it->second.serverPath = "." + formatPath(it->second.alias);
    // Add parent root to location blocks if they don't have a root or alias
    else if (it->second.root.empty() && it->second.alias.empty()
             && serverRoot.size())
    {
      it->second.root = serverRoot;
      it->second.serverPath
          = "." + formatPath(it->second.root) + formatPath(it->first);
    }
  }
}

void ServerConfig::resolveServerPathForErrorPages()
{
  for (std::map<int, std::string>::iterator it = errorPages.begin();
       it != errorPages.end(); it++)
  {
    if (serverPath.size())
      it->second = serverPath + formatPath(it->second);
    // std::cout << "errorPages path on server: " << it->first << " " << it->second
    //           << std::endl;
  }
}