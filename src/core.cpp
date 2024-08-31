/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   core.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: demre <demre@student.42malaga.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/05 19:51:10 by demre             #+#    #+#             */
/*   Updated: 2024/08/30 16:00:22 by demre            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "core.hpp"

bool isDirectory(const std::string &path)
{
  struct stat path_stat;
  if (stat(path.c_str(), &path_stat) == 0)
    return (S_ISDIR(path_stat.st_mode));
  return (false);
}

bool isFile(const std::string &path)
{
  struct stat buffer;
  if (stat(path.c_str(), &buffer) == 0)
    return (S_ISREG(buffer.st_mode));
  return (false);
}

std::vector<std::string> listFilesInDirectory(const std::string &dirPath)
{
  std::vector<std::string> files;

  // std::cout << "DIRPATH: " << dirPath << std::endl;
  DIR *dirp = opendir(dirPath.c_str()); // Pointer to a folder
  if (dirp)
  {
    // Structure used to represent the entries of a directory
    struct dirent *entry;
    while ((entry = readdir(dirp)) != NULL)
    {
      if (entry->d_type == DT_REG
          && entry->d_name[0] != '.') //Verifies if it is a regular file
        files.push_back(entry->d_name);
    }
    closedir(dirp);
  }
  else
    throw HttpException(500, "Failed to open directory " + dirPath);

  return (files);
}

bool findURIorParentFolderInLocations(
    const std::string &URI,
    const std::map<std::string, LocationConfig> &locations,
    std::map<std::string, LocationConfig>::const_iterator &it)
{
  // Initialise the URI to check
  std::string currentURI = URI;

  // Check if the current URI exists in the map container
  it = locations.find(currentURI);
  if (it != locations.end())
    return (true);

  // Trim the end of the URI by removing the last segment
  size_t lastSlashPos = currentURI.find_last_of('/');
  if (lastSlashPos == std::string::npos) // nothing left to trim
    return (false);

  // Update the URI to remove the last segment
  currentURI.erase(lastSlashPos);

  // Check if the updated URI exists in the map container
  if (!currentURI.empty())
    it = locations.find(currentURI);
  if (it != locations.end())
    return (true);

  return (false);
}

bool findURIstartInLocations(
    const std::string &URI,
    const std::map<std::string, LocationConfig> &locations,
    std::map<std::string, LocationConfig>::const_iterator &it)
{
  std::string currentURI = URI;
  std::cout << "Current URI : " << URI << std::endl;
  while (!currentURI.empty())
  {
    // Check if the current URI segment matches any location
    if (!currentURI.empty())
      it = locations.find(currentURI);
    if (it != locations.end())
      return (true);

    // Find the last '/' in the current URI
    size_t lastSlashPos = currentURI.find_last_of('/');
    if (lastSlashPos == std::string::npos) // nothing left to trim
      return (false);

    // Truncate the URI at the last '/' found
    currentURI.erase(lastSlashPos);
  }
  return (false);
}

std::string urlDecode(const std::string &url)
{
  std::string decoded;
  size_t length = url.length();

  for (size_t i = 0; i < length; ++i)
  {
    if (url[i] == '%')
    {
      // Check if there are at least two characters following '%'
      if (i + 2 < length && std::isxdigit(url[i + 1])
          && std::isxdigit(url[i + 2]))
      {
        // Decode the hexadecimal value
        int value = std::strtol(url.substr(i + 1, 2).c_str(), NULL, 16);
        decoded += static_cast<char>(value);
        i += 2; // Skip past the hex digits
      }
      else
        decoded += '%';
    }
    else if (url[i] == '+')
      decoded += ' '; // '+' is used for spaces in query parameters
    else
      decoded += url[i]; // Copy other characters unchanged
  }
  return (decoded);
}

std::string getHttpDate()
{
  // Get the current time
  time_t now = time(0);
  struct tm *gmt = gmtime(&now);

  // Format the date in RFC 1123 format (e.g., "Fri, 23 Aug 2024 14:00:00 GMT")
  char buffer[128];
  strftime(buffer, sizeof(buffer), "%a, %d %b %Y %H:%M:%S GMT", gmt);

  return (std::string(buffer));
}
