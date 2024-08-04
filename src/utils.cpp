/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: demre <demre@student.42malaga.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/31 15:30:21 by blarger           #+#    #+#             */
/*   Updated: 2024/08/04 17:42:59 by demre            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils.hpp"
#include "Dependencies.hpp"

int sendall(int s, const char *buf, int len)
{
  int totalBytesSent = 0; // how many bytes we've sent
  int bytesleft = len;    // how many we have left to send
  int n;

  while (totalBytesSent < len)
  {
    n = send(s, buf + totalBytesSent, bytesleft, 0);
    if (n == -1)
      break;
    totalBytesSent += n;
    bytesleft -= n;
  }

  len = totalBytesSent; // return number actually sent here

  return (n == -1 ? -1 : 0); // return -1 on failure, 0 on success
}

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

bool checkStreamForRemainingContent(std::stringstream &ss)
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

bool hasDuplicates(const std::vector<int> &vec)
{
  for (std::vector<int>::size_type i = 0; i < vec.size(); ++i)
  {
    // Compare the current element with all subsequent elements
    for (std::vector<int>::size_type j = i + 1; j < vec.size(); ++j)
    {
      if (vec[i] == vec[j])
        return (true);
    }
  }
  return (false);
}

bool hasDuplicates(const std::vector<std::string> &vec)
{
  for (std::vector<std::string>::size_type i = 0; i < vec.size(); ++i)
  {
    // Compare the current element with all subsequent elements
    for (std::vector<std::string>::size_type j = i + 1; j < vec.size(); ++j)
    {
      if (vec[i] == vec[j])
        return (true);
    }
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
              << ", index: " << serverConfigs[i].serverIndex << ", server_names"
              << serverConfigs[i].getServerNames() << std::endl;
    std::cout << "  locations.size(" << serverConfigs[i].locations.size()
              << "): " << std::endl;

    for (std::map<std::string, LocationConfig>::iterator it
         = serverConfigs[i].locations.begin();
         it != serverConfigs[i].locations.end(); ++it)
    {
      std::cout << "  location " << it->first << ", root: " << it->second.root
                << ", index: " << it->second.index
                << ", autoIndexOn: " << it->second.autoIndexOn << std::endl;

      for (std::map<int, std::string>::iterator itR
           = it->second.redirection.begin();
           itR != it->second.redirection.end(); ++itR)
      {
        std::cout << "    redirection:  " << itR->first << " " << itR->second
                  << std::endl;
      }

      std::cout << "    allowedMethods: ";
      for (unsigned long itAM = 0; itAM < it->second.allowedMethods.size();
           ++itAM)
      {
        std::cout << it->second.allowedMethods[itAM] << " ";
      }
      std::cout << std::endl;
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
