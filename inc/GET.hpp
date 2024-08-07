/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   GET.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isporras <isporras@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/09 18:24:19 by demre             #+#    #+#             */
/*   Updated: 2024/08/07 17:25:21 by isporras         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#ifndef GET_HPP
#define GET_HPP

#include "ErrorUtils.hpp"
#include "Webserv.hpp"

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
  // ServerConfig
  const ServerConfig &serverConfig;

public:
  GET(int clientFD, std::string &clientInput, const ServerConfig &serverConfig);
  ~GET(void);

  void setHost(const std::string &_host);
  void setUserAgent(const std::string &_userAgent);
  void setAccept(const std::string &_accept);

  void findHeader(std::string &key, std::istringstream &isLine);
  std::string createResponseBody(void);
  std::string handleLocations(std::string pathToResource);

  // Return vector of all file names in folder
  std::vector<std::string> listDirectoryContent(const std::string &path);

  // Generate html page with directory listing
  std::string generateDirectoryListing(
      const std::string &path, const std::vector<std::string> &contents);
};

#endif