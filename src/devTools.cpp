/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   devTools.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: demre <demre@student.42malaga.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/13 12:32:49 by demre             #+#    #+#             */
/*   Updated: 2024/09/02 13:53:02 by demre            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ClientInfo.hpp"
#include "core.hpp"
#include "utils.hpp"

void displayServerConfigs(std::vector<ServerConfig> const &serverConfigs)
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
              << ", serverPath: " << serverConfigs[i].serverPath
              << ", index: " << serverConfigs[i].serverIndex << ", server_names"
              << serverConfigs[i].getServerNames() << std::endl;

    std::cout << "  errorPages.size(" << serverConfigs[i].errorPages.size()
              << "): ";
    for (std::map<int, std::string>::const_iterator it
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
    for (std::map<std::string, LocationConfig>::const_iterator it
         = serverConfigs[i].locations.begin();
         it != serverConfigs[i].locations.end(); ++it)
    {
      std::cout << "    location  " << it->first
                << ", root: " << it->second.root
                << ", index:  " << it->second.index
                << ", alias:  " << it->second.alias
                << ", serverPath:  " << it->second.serverPath
                << ", autoIndexOn:  " << it->second.autoIndexOn
                << ", allowedMethods: ";
      for (unsigned long itAM = 0; itAM < it->second.allowedMethods.size();
           ++itAM)
      {
        std::cout << it->second.allowedMethods[itAM] << " ";
      }
      std::cout << std::endl;

      std::cout << "      redirection:  ";
      if (it->second.redirection.first)
        std::cout << it->second.redirection.first << " "
                  << it->second.redirection.second;
      std::cout << std::endl;
    }
  }
}

// Display client request location data in the terminal
void displayClientRequestLocationData(ClientInfo const &client)
{
  std::cout << "URIpath: " << client.req.URIpath
            << ", pathFolder: " << client.req.pathFolder
            << ", pathOnServer: " << client.req.pathOnServer << ", isDir "
            << isDirectory(client.req.pathOnServer) << ", isFile "
            << isFile(client.req.pathOnServer)
            << ", pathFolderOnServer: " << client.req.pathFolderOnServer
            << ", isDir " << isDirectory(client.req.pathFolderOnServer)
            << ", isFile " << isFile(client.req.pathFolderOnServer)
            << std::endl;
}

// Display parsed header request in the terminal
void displayParsedHeaderRequest(ClientInfo const &client)
{
  std::cout << std::endl
            << MAGENTA << client.req.method << " " << client.req.URIpath << " "
            << client.req.HTTPversion << std::endl;
  for (std::map<std::string, std::string>::const_iterator it
       = client.req.fields.begin();
       it != client.req.fields.end(); it++)
  {
    std::cout << it->first << ": " << it->second << std::endl;
  }
  std::cout << RESET << std::endl;
}