/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   GET.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blarger <blarger@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/09 18:24:19 by demre             #+#    #+#             */
/*   Updated: 2024/08/11 17:23:54 by blarger          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "ErrorUtils.hpp"
#include "Webserv.hpp"

class GET
{

private:
  // ServerConfig
  const ServerConfig &serverConfig;

public:
  GET(ClientInfo &client, int clientFD, std::string &clientInput,
      const ServerConfig &serverConfig);
  ~GET(void);

  std::string getResponseAtLocation(ClientRequest &req);

  // Return vector of all file names in folder
  std::vector<std::string> listDirectoryContent(const std::string &path);

  // Generate html page with directory listing
  std::string generateDirectoryListing(
      const std::string &path, const std::vector<std::string> &contents);

  // Generate html page with delete endpoint
  std::string createHtmlDeleteRequest(std::vector<std::string> files,
                                      std::string uploadspth);

  // Manage delete endpoint
  std::string manageDeleteEndPoint();
};
