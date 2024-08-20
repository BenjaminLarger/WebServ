/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   WebservClientRequestLocation.cpp                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blarger <blarger@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: Invalid date        by                   #+#    #+#             */
/*   Updated: 2024/08/20 09:37:31 by blarger          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Webserv.hpp"
#include "core.hpp"

static std::string replaceUriPrefix(const std::string &uri,
                                    const std::string &prefix,
                                    const std::string &replacement)
{
  // Check if the URI starts with the prefix
  if (uri.compare(0, prefix.length(), prefix) == 0)
  {
    // Replace the prefix with the replacement string
    return (replacement + uri.substr(prefix.length()));
  }
  // If the prefix does not match, return the original URI
  return (uri);
}

void Webserv::resolveRequestedPathFromLocations(
    ClientRequest &req, const ServerConfig &serverConfig)
{
  std::map<std::string, LocationConfig> locations = serverConfig.locations;
  std::map<std::string, LocationConfig>::const_iterator it;

  // Looks for a direct match in locations or a match of any parent folder
  // if (!findURIorParentFolderInLocations(req.URIpath, locations, it))
  findURIstartInLocations(req.URIpath, locations, it);
  // if URI or parent folder found
  if (it != locations.end())
  {
    if (it->second.alias.size())
    {
      std::cout << "replacing with alias" << std::endl;
      req.pathFolder = it->first == "/" ? "/" : formatPath(it->first);
      req.pathOnServer
          = replaceUriPrefix(req.URIpath, it->first, it->second.serverPath);
      req.pathFolderOnServer = it->second.serverPath;
			std::cout << RED << "req.pathOnServer = " << req.pathOnServer << RESET << std::endl;
    }
    else if (it->second.root.size())
    {
      std::cout << "replacing with root" << std::endl;

      req.pathFolder = it->first == "/" ? "/" : formatPath(it->first);
      req.pathOnServer
          = "." + formatPath(it->second.root) + formatPath(req.URIpath);
      req.pathFolderOnServer
          = "." + formatPath(it->second.root) + formatPath(it->first);
				std::cout << RED << "req.pathOnServer = " << req.pathOnServer << RESET << std::endl;
    }
  }
  else
  {
    throw HttpException(404, "Not Found: Location not found.");
  }
}

bool Webserv::isMethodAllowedAtLoc(ClientRequest &req,
                                   const ServerConfig &serverConfig)
{
  if (serverConfig.locations.find(req.pathFolder)
      != serverConfig.locations.end())
  {
    LocationConfig loc = serverConfig.locations.find(req.pathFolder)->second;
    if (std::find(loc.allowedMethods.begin(), loc.allowedMethods.end(),
                  req.method)
        != loc.allowedMethods.end())
      return (true);
  }
  return (false);
}