/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   WebservClientRequestLocation.cpp                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: demre <demre@student.42malaga.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/01 20:07:09 by demre             #+#    #+#             */
/*   Updated: 2024/08/08 19:41:09 by demre            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Webserv.hpp"
#include "core.hpp"

std::string pathToRessource;    // /new/new.png
std::string pathParentFolder;   // /new
std::string pathOnServer;       // ./var/www/new/new.png
std::string pathParentOnServer; //./var/www/new

void Webserv::handleLocations(ClientRequest &req,
                              const ServerConfig &serverConfig)
{

  std::string response;
  std::map<std::string, LocationConfig> locations = serverConfig.locations;
  std::map<std::string, LocationConfig>::const_iterator it;

  pathOrParentFolderExistsInLocations(req.pathToRessource, locations, it);

  if (it != locations.end())
  {
    std::string root = it->second.root;
    req.pathFolder = it->first;
    req.pathOnServer = "." + root + req.pathToRessource;
    req.pathFolderOnServer = "." + root + it->first;

    std::cout << "pathToRessource: " << req.pathToRessource
              << ", pathToRessource: " << req.pathFolder
              << ", pathOnServer: " << req.pathOnServer << ", isDir "
              << isDirectory(req.pathOnServer) << ", isFile "
              << isFile(req.pathOnServer)
              << ", pathFolderOnServer: " << req.pathFolderOnServer
              << ", isDir " << isDirectory(req.pathFolderOnServer)
              << ", isFile " << isFile(req.pathFolderOnServer) << std::endl;

    // If is not in locations and is not an existing file or a folder, 404
    // if (req.pathToRessource != req.pathFolder && !isDirectory(req.pathOnServer)
    //     && !isFile(req.pathOnServer))
    //   throw HttpException(404, "Not Found");
  }
  else
  {
    throw HttpException(404, "Not Found");
  }
}

bool isMethodAllowedAtLoc(std::string method, ClientRequest &req,
                          const ServerConfig &serverConfig)
{
  // std::string locToCheck;
  // if (client.req.pathToRessource != client.req.pathFolder
  //     && !isDirectory(client.req.pathOnServer))
  //   locToCheck;
  // std::vector<std::string> allowedMethods;

  return (true);
}