/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blarger <blarger@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: Invalid date        by                   #+#    #+#             */
/*   Updated: 2024/08/13 13:18:04 by blarger          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils.hpp"
#include "core.hpp"

bool isAllWhitespace(const std::string &str)
{
  for (std::string::const_iterator it = str.begin(); it != str.end(); ++it)
  {
    if (!std::isspace(*it))
      return (false);
  }
  return (true);
}

std::string toString(int num)
{
  std::ostringstream oss;
  oss << num;
  return (oss.str());
}
std::string toString(size_t num)
{
  std::ostringstream oss;
  oss << num;
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

std::string extractFirstWord(const std::string &str)
{
  std::istringstream stream(str);
  std::string firstWord;

  stream >> firstWord;
  return (firstWord);
}

bool lineIsEmpty(std::string line)
{
  for (int i = 0; line[i]; i++)
  {
    if ((line[i] < 9 || line[i] > 13) && line[i] != 32)
      return (false);
  }
  return (true);
}

std::string trimQuotes(std::string &str)
{
  trimBothEnds(str);
  std::cout << RED << "len = " << str.length() << ", [0] = " << str[0]
            << ", [end] = " << str[str.length() - 1] << RESET << std::endl;
  if (str.length() >= 2 && str[0] == '"' && str[str.length() - 1] == '"')
  {
    str = str.substr(1, str.length() - 2);
  }
  return (str);
}

int countJumpLine(std::string str)
{
  int count = 0;

  for (int i = 0; str[i]; i++)
  {
    if (str[i] == '\n')
      count++;
  }
  return (count);
}

std::string getCurrentTimeHttpFormat()
{
  std::time_t now = std::time(NULL);

  // Convert it to UTC
  std::tm *gmt = std::gmtime(&now);

  char buffer[100];
  std::strftime(buffer, sizeof(buffer), "%a, %d %b %Y %H:%M:%S GMT", gmt);

  // Devolver el string resultante
  return std::string(buffer);
}

std::string formatPath(const std::string &str)
{
  std::string path = str;
  // Check if path starts with "/" and add one if not
  if (!path.empty() && path[0] != '/')
    path.insert(path.begin(), '/');

  // Check if path ends with "/" and remove it if yes
  if (!path.empty() && path[path.size() - 1] == '/')
    path.erase(path.size() - 1);

  return (path);
}

void getFileNameAndExtension(const std::string &path, std::string &fileName,
                             std::string &extension)
{
  std::string::size_type lastSlash = path.find_last_of('/');
  if (lastSlash == std::string::npos) // No '/' found
  {
    fileName = "", extension = "";
    return;
  }

  // Extract the substring after the last '/'
  fileName = path.substr(lastSlash + 1);

  std::string::size_type lastDot = fileName.find_last_of('.');
  if (lastDot == std::string::npos) // No '.' found
  {
    extension = "";
    return;
  }

  // Extract the extension after the last '.'
  extension = fileName.substr(lastDot + 1);
  fileName = fileName.substr(0, lastDot);
}

bool hasBlankLineInput(std::string &clientInput)
{
  //Find if it is a POST request
  size_t isPost = clientInput.rfind("POST /submit-form HTTP/1.1");
  if (isPost != std::string::npos)
  {
    std::cout << YELLOW << "Is a post request\n" << RESET << std::endl;
    return (true);
  }

  // Find the position of the last newline character
  size_t lastNewlinePos = clientInput.rfind('\n');

  // If there is no newline character, return false
  if (lastNewlinePos == std::string::npos || clientInput.size() <= 3)
  {
    std::cout << YELLOW << "No newline character found." << RESET << std::endl;
    return (false);
  }

  // If the last newline character is at the beginning, return false
  if (lastNewlinePos == 0)
  {
    std::cout << YELLOW << "The last newline character is at the beginning."
              << RESET << std::endl;
    return (false);
  }

	std::cout << "clientInput[lastNewlinePos - 2] " << (int)clientInput[lastNewlinePos - 2] << "clientInput[lastNewlinePos - 1] = " << (int)clientInput[lastNewlinePos - 1] << RESET << std::endl;
  if (lastNewlinePos > 2 && ((clientInput[lastNewlinePos - 2] == '\r'
      && clientInput[lastNewlinePos - 1] == '\n') || (clientInput[lastNewlinePos - 2] == '\n' && clientInput[lastNewlinePos - 1] == '\r')))
  {
    std::cout << YELLOW
              << "The last newline character is preceded by another newline "
                 "character."
              << RESET << std::endl;
    return true;
  }
  if (clientInput[clientInput.size() - 1] == '\n'
      && clientInput[clientInput.size() - 2] == '\r'
      && clientInput[clientInput.size() - 3] == '\n')
    return (true);

  return (false);
}