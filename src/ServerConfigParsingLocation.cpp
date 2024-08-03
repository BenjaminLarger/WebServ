/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerConfigParsingLocation.cpp                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: demre <demre@student.42malaga.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/02 18:23:26 by demre             #+#    #+#             */
/*   Updated: 2024/08/03 19:19:01 by demre            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ServerConfig.hpp"
#include "utils.hpp"

void ServerConfig::parseLocation(std::ifstream &file, std::string urlPattern)
{
  std::string line;

  // std::cout << "urlPattern: '" << urlPattern << "'" << std::endl;

  while (std::getline(file, line))
  {
    trimTrailingWS(line);
    // std::cout << "line: '" << line << "'" << std::endl;
    std::stringstream ss(line);
    std::string key, valueStr;
    ss >> key;

    // Prevent nested location blocks
    if (key == "location")
      file.close(), throw(std::runtime_error(
                        "Invalid location block in config file: " + line));

    validateAndSanitizeLocationLine(line, ss, key, file);

    if (key.size() && key[0] == '#')
      continue;
    else if (key == "limit_except") // list of accepted HTTP methods
    {
      while (ss >> valueStr)
      {
        if (valueStr != "GET" && valueStr != "POST" && valueStr != "DELETE")
          file.close(),
              throw(std::runtime_error(
                  "Not a valid HTTP method in location block: " + line));

        if (!isAllWhitespace(valueStr))
          this->locations[urlPattern].allowedMethods.push_back(valueStr);
      }
    }
    else if (key == "return") // HTTP redirection
    {
      int valueInt;
      ss >> valueInt >> valueStr;
      if (!valueStr.size() || checkStreamForRemainingContent(ss)
          || (valueInt != 301 && valueInt != 302 && valueInt != 303
              && valueInt != 307 && valueInt != 308))
        file.close(),
            throw(std::runtime_error(
                "Incorrect HTTP redirection in location block: " + line));

      this->locations[urlPattern].redirection[valueInt] = valueStr;
    }
    else if (key == "root") // where the file should be searched
    {
      ss >> valueStr;
      if (!valueStr.size() || checkStreamForRemainingContent(ss))
        file.close(), throw(std::runtime_error(
                          "Unexpected characters in location block: " + line));

      this->locations[urlPattern].root = valueStr;
    }
    // What is Directory Listing?
    // Directory listing is a feature that allows the server to display the contents of a directory if no specific file is requested. When enabled, accessing a directory URL without a specified file will list all files and subdirectories within that directory.
    else if (key == "autoindex")
    {
      ss >> valueStr;
      if ((valueStr != "on" && valueStr != "off")
          || checkStreamForRemainingContent(ss))
        file.close(),
            throw(std::runtime_error(
                "Incorrect directory listing data in location block: " + line));

      if (valueStr == "on")
        this->locations[urlPattern].autoIndexOn = true;
      else
        this->locations[urlPattern].autoIndexOn = false;
    }
    else if (key == "index") // default file if the request is a directory
    {
      ss >> valueStr;
      if (!valueStr.size() || checkStreamForRemainingContent(ss))
        file.close(), throw(std::runtime_error(
                          "Unexpected characters in location block: " + line));

      this->locations[urlPattern].index = valueStr;
    }
    else if (key.size() && key[0] == '}')
      break;
    else if (key.size() && !isAllWhitespace(key))
      file.close(),
          throw(std::runtime_error("Invalid data in config file: " + key));
  }
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
