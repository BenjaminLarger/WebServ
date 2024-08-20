/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   core.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blarger <blarger@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/05 19:51:10 by demre             #+#    #+#             */
/*   Updated: 2024/08/16 11:46:55 by blarger          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "core.hpp"

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

  len = totalBytesSent;      // return number actually sent here
  return (n == -1 ? -1 : 0); // return -1 on failure, 0 on success
}

void sendRGeneric(int clientFD, std::string responseStr)
{
  if (sendall(clientFD, responseStr.c_str(), responseStr.size()) == -1)
    throw HttpException(
        500, strerror(errno));
}

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
      if (entry->d_type == DT_REG && entry->d_name[0] != '.') //Verifies if it is a regular file
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