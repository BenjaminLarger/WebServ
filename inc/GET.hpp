/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   GET.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: demre <demre@student.42malaga.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/09 18:24:19 by demre             #+#    #+#             */
/*   Updated: 2024/08/10 13:45:30 by demre            ###   ########.fr       */
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

  std::string handleLocations(std::string URI);

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
