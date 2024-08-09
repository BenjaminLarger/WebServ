/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   core.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isporras <isporras@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/05 19:51:10 by demre             #+#    #+#             */
/*   Updated: 2024/08/09 13:48:59 by isporras         ###   ########.fr       */
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
        500, "Internal Server Error: Data failed to be sent to the client");
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
      if (entry->d_type == DT_REG) //Verifies if it is a regular file
        files.push_back(entry->d_name);
    }
    closedir(dirp);
  }
  else
    throw HttpException(500, "Failed to open directory " + dirPath);

  return (files);
}

bool pathOrParentFolderExistsInLocations(
    const std::string &pathToResource,
    const std::map<std::string, LocationConfig> &locations,
    std::map<std::string, LocationConfig>::const_iterator &it)
{
  // Initialize the path to check
  std::string currentPath = pathToResource;

  // Check if the current path exists in the map
  it = locations.find(currentPath);
  if (it != locations.end())
    return (true);

  // Trim the path by removing the last segment
  size_t lastSlashPos = currentPath.find_last_of('/');
  if (lastSlashPos == std::string::npos) // nothing left to trim
    return (false);

  // Update the currentPath to remove the last segment
  currentPath.erase(lastSlashPos);

  if (!currentPath.empty())
    it = locations.find(currentPath);
  if (it != locations.end())
    return (true);

  return (false);
}
