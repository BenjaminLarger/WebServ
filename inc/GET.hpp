/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   GET.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isporras <isporras@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/09 18:24:19 by demre             #+#    #+#             */
/*   Updated: 2024/08/07 13:39:02 by isporras         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#ifndef GET_HPP
#define GET_HPP

#include "ErrorUtils.hpp"
#include "Webserv.hpp"
#include "HttpExceptions.hpp"

class GET
{

private:
  // Request line
  std::string pathToRessource;
  std::string HTTPversion;
  // Header
  std::string host;
  std::string userAgent;
  std::string accept;
  // Body
  std::string body;
  // ServerConfigs
  const std::vector<ServerConfig> &serverConfigs;
  int serverIndex;

public:
  GET(size_t serverIndex, int clientFD, std::string &clientInput,
      const std::vector<ServerConfig> &serverConfigs);
  ~GET(void);

  void setHost(const std::string &_host);
  void setUserAgent(const std::string &_userAgent);
  void setAccept(const std::string &_accept);

  void findHeader(std::string &key, std::istringstream &isLine);
  std::string createResponseBody(void);
  std::string extractHtmlContent(const std::string &filePath);
  std::string handleLocations(std::string pathToResource);

  // Return vector of all files names in folder
  std::vector<std::string> listDirectoryContent(const std::string &path);

  // Generate html page with directory listing
  std::string generateDirectoryListing(
      const std::string &path, const std::vector<std::string> &contents);
};

#endif