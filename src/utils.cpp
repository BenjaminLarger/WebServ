/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blarger <blarger@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: Invalid date        by                   #+#    #+#             */
/*   Updated: 2024/09/03 18:37:40 by blarger          ###   ########.fr       */
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

std::string extractBoundary(const std::string &input, std::string &boundary)
{
  std::size_t pos = input.find('=');
  if (pos != std::string::npos)
  {
    boundary = input.substr(pos + 1);
    std::size_t endPos = boundary.find('\r');
    if (endPos != std::string::npos)
    {
      trimBothEnds(boundary);
      return ("--" + boundary.substr(0, endPos));
    }
    trimBothEnds(boundary);
    return ("--" + boundary);
  }
  return ("");
}

bool isPostCGIrequest(std::string &clientInput)
{
  std::istringstream stream(clientInput);
  std::string firstLine;

  std::getline(stream, firstLine);
  size_t isPost = firstLine.rfind("POST");
  if (isPost != std::string::npos)
  {
    size_t isCGI = firstLine.rfind(".py HTTP/");
    if (isCGI != std::string::npos)
      return (true);
  }
  return (false);
}

bool hasNoInputFields(std::string &clientInput)
{
  std::istringstream stream(clientInput);
  std::string line;
  std::string lastLine;

  while (std::getline(stream, line))
    lastLine = line;

  if (lastLine.find('=') != std::string::npos)
    return (false);
  else
    return (true);
}
bool hasBlankLineInput(std::string &clientInput, std::string &boundary,
                       ClientInfo &client)
{
  //Find if it is a POST submit-form request
  if (!strncmp(clientInput.c_str(), "POST /", 6))
  {
    if (extractBoundary(client.req.fields["Content-Type"], boundary).size())
    {
      size_t isFinalBoundary = clientInput.rfind(boundary + "--");
      if (isFinalBoundary != std::string::npos)
        return (true);
      else
        return (false);
    }
  }

  //Find if it is a POST request
  if (isPostCGIrequest(clientInput) == true
      && hasNoInputFields(clientInput) == true)
    return (false);

  // Find the position of the last newline character
  size_t lastNewlinePos = clientInput.rfind('\n');

  // If there is no newline character, return false
  if (lastNewlinePos == std::string::npos || clientInput.size() <= 3)
    return (false);

  // If the last newline character is at the beginning, return false
  if (lastNewlinePos == 0)
    return (false);

  if (lastNewlinePos > 2
      && ((clientInput[lastNewlinePos - 2] == '\r'
           && clientInput[lastNewlinePos - 1] == '\n')
          || (clientInput[lastNewlinePos - 2] == '\n'
              && clientInput[lastNewlinePos - 1] == '\r')))
    return true;
  if (clientInput[clientInput.size() - 1] == '\n'
      && clientInput[clientInput.size() - 2] == '\r'
      && clientInput[clientInput.size() - 3] == '\n')
    return (true);
  return (false);
}

void deleteLogContentFile(const std::string &filePath)
{

  // Open the file in write mode to truncate it
  std::fstream ofs(filePath.c_str(), std::ofstream::out | std::ofstream::trunc);
  if (!ofs)
  {
    std::cerr << "Error opening file to erase content: " << filePath
              << std::endl;
  }
  else
    ofs.close();
}

std::string getHttpDate()
{
  // Get the current time and convert to GMT
  std::time_t now = std::time(NULL);
  std::tm *gmt = std::gmtime(&now);

  // Format the date in RFC 1123 format (e.g., "Fri, 23 Aug 2024 14:00:00 GMT")
  char buffer[128];
  std::strftime(buffer, sizeof(buffer), "%a, %d %b %Y %H:%M:%S GMT", gmt);

  return (std::string(buffer));
}

std::string getLogDate()
{
  // Get the current time and convert to GMT
  std::time_t now = std::time(NULL);
  std::tm *gmt = std::gmtime(&now);

  // Format the date (e.g., "2024-09-03 15:32:11 GMT")
  char buffer[128];
  std::strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S GMT", gmt);

  return (std::string(buffer));
}
