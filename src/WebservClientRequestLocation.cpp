/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   WebservClientRequestLocation.cpp                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isporras <isporras@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: Invalid date        by                   #+#    #+#             */
/*   Updated: 2024/08/10 17:27:10 by isporras         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Webserv.hpp"
#include "core.hpp"

std::string URI;                // /new/new.png
std::string pathParentFolder;   // /new
std::string pathOnServer;       // ./var/www/new/new.png
std::string pathParentOnServer; //./var/www/new

void Webserv::resolveLocations(ClientRequest &req,
                               const ServerConfig &serverConfig)
{
  std::map<std::string, LocationConfig> locations = serverConfig.locations;
  std::map<std::string, LocationConfig>::const_iterator it;

  pathOrParentFolderExistsInLocations(req.URI, locations, it);

  // if
  if (it != locations.end())
  {
    std::string root = it->second.root;
    req.pathFolder = it->first;
    req.pathOnServer = "." + root + req.URI;
    req.pathFolderOnServer = "." + root + it->first;

    std::cout << "URI: " << req.URI << ", URI: " << req.pathFolder
              << ", pathOnServer: " << req.pathOnServer << ", isDir "
              << isDirectory(req.pathOnServer) << ", isFile "
              << isFile(req.pathOnServer)
              << ", pathFolderOnServer: " << req.pathFolderOnServer
              << ", isDir " << isDirectory(req.pathFolderOnServer)
              << ", isFile " << isFile(req.pathFolderOnServer) << std::endl;

    // If is not in locations and is not an existing file or a folder, 404
    // if (req.URI != req.pathFolder && !isDirectory(req.pathOnServer)
    //     && !isFile(req.pathOnServer))
    //   throw HttpException(404, "Not Found");

    // Add parent root to location blocks if they don't have a root or alias
    // if (serverConfig.serverRoot.size())
    // {
    //   for (std::map<std::string, LocationConfig>::const_iterator it
    //        = serverConfig.locations.begin();
    //        it != serverConfig.locations.end(); it++)
    //   {
    //     if (it->second.root.empty() && it->second.alias.empty())
    //     {
    //       it->second.root = serverConfig.serverRoot;
    //       std::cout << it->first << ": " << it->second.root << std::endl;
    //     }
    //   }
    // }
  }
  else
  {
    throw HttpException(404, "Not Found: Location not found.");
  }
}

bool Webserv::isMethodAllowedAtLoc(ClientRequest &req,
                          const ServerConfig &serverConfig)
{
  if (serverConfig.locations.find(req.pathFolder) != serverConfig.locations.end())
  {
    LocationConfig loc = serverConfig.locations.find(req.pathFolder)->second;
    if (std::find(loc.allowedMethods.begin(), loc.allowedMethods.end(), req.method) != loc.allowedMethods.end())
          return (true);
  }
  return (false);
}