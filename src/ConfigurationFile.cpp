/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigurationFile.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: demre <demre@student.42malaga.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/03 11:48:15 by blarger           #+#    #+#             */
/*   Updated: 2024/07/09 18:14:58 by demre            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Webserv.hpp"

/* --------------CONSTRUCTORS */
ServerConfig::ServerConfig(){};

ServerConfig::ServerConfig(const char *filename)
{
  std::map<std::string, std::string> options;
  std::ifstream file(filename);
  std::string currentLocation;

  if (!file.is_open())
    throw(std::runtime_error("Could not open the configuration file!"));

  for (std::string line; std::getline(file, line);)
  {
    std::istringstream isLine(line);
    std::string key;

    isLine >> key;
    std::cout << "----------------------------------------------\n";
    std::cout << BLUE << "line = " << line << std::endl;
    std::cout << ORANGE << "key = " << key << RESET << std::endl;
    if (key == "server")
    {
      isLine >> key;
      std::cout << ORANGE << "key = " << key
                << ", key len = " << strlen(key.c_str()) << RESET << std::endl;
    }
    if (key == "{")
    {
      isLine >> key;
      std::cout << ORANGE << "key = " << key << RESET << std::endl;
    }
    if (key[0] == '#')
      continue;
    else if (key == "client_max_body_size")
    {
      isLine >> this->maxBodySize;
      std::cout << "Max body size = " << YELLOW << this->maxBodySize << RESET
                << std::endl;
    }
    else if (key == "listen")
    {
      isLine >> this->port;
      std::cout << "port = " << YELLOW << this->port << RESET << std::endl;
    }
    else if (key == "server_name")
    {
      isLine >> this->host;
      std::cout << "host = " << YELLOW << this->host << RESET << std::endl;
    }
    else if (key == "error_page")
      this->errorPages = findErrorPage(isLine);
    else if (key == "location")
    {
      isLine >> currentLocation;
      findLocation(line, *this, file, currentLocation);
    }
  }
  Webserv server(filename, *this);
}

/* --------------COPY */
/* ServerConfig::ServerConfig(const ServerConfig& other) : port(other.port),
host(other.host), maxBodySize(other.maxBodySize)
{} */

/* --------------GETTER */
const unsigned int &ServerConfig::getPort(void) const { return (this->port); }
