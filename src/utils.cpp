/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blarger <blarger@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/31 15:30:21 by blarger           #+#    #+#             */
/*   Updated: 2024/08/06 16:19:49 by blarger          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils.hpp"

bool isAllWhitespace(const std::string &str)
{
  for (std::string::const_iterator it = str.begin(); it != str.end(); ++it)
  {
    if (!std::isspace(*it))
      return (false);
  }
  return (true);
}

std::string intToString(int value)
{
  std::ostringstream oss;
  oss << value;
  return (oss.str());
}

bool streamHasRemainingContent(std::stringstream &ss)
{
  std::string remaining;
  ss >> remaining;
  if (!remaining.empty() && remaining[remaining.size() - 1] == ';')
    remaining.erase(remaining.size() - 1); // Remove trailing semicolon
  // std::cout << "remaining: '" << remaining << "'" << std::endl;
  if (!remaining.empty())
    return (true);
  return (false);
}

bool streamHasRemainingContent(std::istringstream &iss)
{
  std::string remaining;
  iss >> remaining;
  if (!remaining.empty() && remaining[remaining.size() - 1] == ';')
    remaining.erase(remaining.size() - 1); // Remove trailing semicolon
  if (!remaining.empty())
    return (true);
  return (false);
}

void trimBothEnds(std::string &line)
{
  // Trim leading whitespace
  std::string::size_type start = line.find_first_not_of(" \t\n\r\f\v");
  if (start == std::string::npos)
  {
    // No non-whitespace characters found; the string is all whitespace
    line = "";
    return;
  }

  // Trim trailing whitespace
  std::string::size_type end = line.find_last_not_of(" \t\n\r\f\v");
  line = line.substr(start, end - start + 1);
}

void trimTrailingWS(std::string &line)
{
  std::string::size_type end = line.find_last_not_of(" \t\n\r\f\v");
  line = line.substr(0, end + 1);
}

bool hasDuplicates(const std::vector<int> &container)
{
  for (std::vector<int>::size_type i = 0; i < container.size(); ++i)
  {
    // Compare the current element with all subsequent elements
    for (std::vector<int>::size_type j = i + 1; j < container.size(); ++j)
    {
      if (container[i] == container[j])
        return (true);
    }
  }
  return (false);
}

bool hasDuplicates(const std::vector<std::string> &container)
{
  for (std::vector<std::string>::size_type i = 0; i < container.size(); ++i)
  {
    for (std::vector<std::string>::size_type j = i + 1; j < container.size();
         ++j)
    {
      if (container[i] == container[j])
        return (true);
    }
  }
  return (false);
}

bool hasDuplicates(const std::map<int, std::string> &container, int value)
{
  std::map<int, std::string>::const_iterator it;

  for (it = container.begin(); it != container.end(); ++it)
  {
    if (it->first == value)
      return (true);
  }
  return (false);
}

void displayServerConfigs(std::vector<ServerConfig> &serverConfigs)
{
  std::cout << "serverConfigs.size(): " << serverConfigs.size() << std::endl;
  std::cout << std::endl;

  for (size_t i = 0; i < serverConfigs.size(); i++)
  {
    std::cout << "ServerConfig " << i
              << ", host: " << serverConfigs[i].getHost()
              << ", port: " << serverConfigs[i].getPort()
              << ", maxBodySize: " << serverConfigs[i].maxBodySize
              << ", root: " << serverConfigs[i].serverRoot
              << ", index: " << serverConfigs[i].serverIndex
              << ", alias: " << serverConfigs[i].serverAlias << ", server_names"
              << serverConfigs[i].getServerNames() << std::endl;

    std::cout << "  errorPages.size(" << serverConfigs[i].errorPages.size()
              << "): ";
    for (std::map<int, std::string>::iterator it
         = serverConfigs[i].errorPages.begin();
         it != serverConfigs[i].errorPages.end(); ++it)
    {
      std::cout << it->first << " " << it->second;
      if (it != --serverConfigs[i].errorPages.end())
        std::cout << ", ";
    }
    std::cout << std::endl;

    std::cout << "  locations.size(" << serverConfigs[i].locations.size()
              << "): " << std::endl;
    for (std::map<std::string, LocationConfig>::iterator it
         = serverConfigs[i].locations.begin();
         it != serverConfigs[i].locations.end(); ++it)
    {
      std::cout << "    location  " << it->first
                << ", root: " << it->second.root
                << ", index:  " << it->second.index
                << ", alias:  " << it->second.alias
                << ", autoIndexOn:  " << it->second.autoIndexOn
                << ", allowedMethods: ";
      for (unsigned long itAM = 0; itAM < it->second.allowedMethods.size();
           ++itAM)
      {
        std::cout << it->second.allowedMethods[itAM] << " ";
      }
      std::cout << std::endl;

      for (std::map<int, std::string>::iterator itR
           = it->second.redirection.begin();
           itR != it->second.redirection.end(); ++itR)
      {
        std::cout << "      redirection:  " << itR->first << " " << itR->second
                  << std::endl;
      }
    }
    std::cout << std::endl;
  }
}

std::string extractFirstWord(const std::string &str)
{
  std::istringstream stream(str);
  std::string firstWord;

  stream >> firstWord;
  return (firstWord);
}

bool	lineIsEmpty(std::string line)
{
	for (int i = 0; line[i]; i++)
	{
		std::cout << (int)line[i] << ", ";
		if ((line[i] < 9 || line[i] > 13) && line[i] != 32)
			return (false);
	}
	std::cout << std::endl;
	std::cout << ORANGE << "Line is empty!\n" << RESET;
	return (true);
}