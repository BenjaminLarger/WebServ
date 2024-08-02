/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerConfigParsingLocation.cpp                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: demre <demre@student.42malaga.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/02 18:23:26 by demre             #+#    #+#             */
/*   Updated: 2024/08/02 20:45:51 by demre            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ServerConfig.hpp"
#include "utils.hpp"

void ServerConfig::parseLocation(std::ifstream &file, std::string urlPattern)
{
  std::string line;

  std::cout << "urlPattern: '" << urlPattern << "'" << std::endl;

  while (std::getline(file, line))
  {
    trimTrailingWS(line);
    // std::cout << "line: '" << line << "'" << std::endl;
    std::stringstream ss(line);
    std::string key;
    ss >> key;

    // Prevent nested location blocks
    if (key == "location")
      file.close(), throw(std::runtime_error(
                        "Invalid location block in config file: " + line));

    validateAndSanitizeLocationLine(line, ss, key, file);

    if (key.size() && key[0] == '#')
      continue;
    else if (key == "root")
    { /* 
      ss >> config.locations[urlPattern].root; */
    }
    else if (key == "index")
    { /* 
      std::istringstream ss(line);
      std::string index;
      ss >> index;
      ss >> index;

      config.locations[urlPattern].index = index; */
    }
    else if (key == "return")
    {
      // to implement (redirection)
    }
    // What is Directory Listing?
    // Directory listing is a feature that allows the server to display the contents of a directory if no specific file is requested. When enabled, accessing a directory URL without a specified file will list all files and subdirectories within that directory.
    else if (key == "autoindex")
    { /* 
      std::string autoindexState;
      ss >> autoindexState;
      if (autoindexState == "on")
        config.locations[urlPattern].autoindexOn = true;
      else
        config.locations[urlPattern].autoindexOn = false; */
    }
    else if (key == "allow_methods")
    { /* 
      std::istringstream ss(line);
      std::string method;
      ss >> method;
      while (true)
      {
        if (method == "allow_methods")
          ss >> method;

        if (method[strlen(method.c_str()) - 1] == ','
            || method[strlen(method.c_str()) - 1] == ';'
            || method[strlen(method.c_str()) - 1] == ';'
            || method[strlen(method.c_str()) - 1] == '}'
            || method[strlen(method.c_str()) - 1] == ' ')
          method[strlen(method.c_str()) - 1] = '\0';

        if (strncmp(method.c_str(), "POST", 4)
            && strncmp(method.c_str(), "GET", 3)
            && strncmp(method.c_str(), "DELETE", 6))
          break;
        config.locations[urlPattern].allowedMethods.push_back(method);

        std::string prevMethod = method;
        ss >> method;

        if (method == prevMethod)
          break;
      } */
    }
    else if (key.size() && key[0] == '}')
      break;
    else if (key.size() && !isAllWhitespace(key))
      file.close(),
          throw(std::runtime_error("Invalid data in config file: " + key));
  }

  this->locations[urlPattern] = LocationConfig(); // does not exist
  //std::cout << "config.locations[urlPattern] = " << YELLOW
  //<< config.locations[urlPattern].root << RESET << std::endl;
}

void ServerConfig::validateAndSanitizeLocationLine(std::string &line,
                                                   std::stringstream &ss,
                                                   std::string &key,
                                                   std::ifstream &file)
{
  // Check line ends correctly
  if (line.size() && line[line.size() - 1] != ';' && key.size() && key[0] != '#'
      && key != "}")
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
