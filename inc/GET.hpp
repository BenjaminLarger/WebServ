/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   GET.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: demre <demre@student.42malaga.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/09 18:24:19 by demre             #+#    #+#             */
/*   Updated: 2024/08/05 19:18:25 by demre            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#ifndef GET_HPP
#define GET_HPP

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

public:
  GET(int serverFD, int clientFD, std::string &clientInput);
  GET();
  ~GET(void);

  void setHost(const std::string &_host);
  void setUserAgent(const std::string &_userAgent);
  void setAccept(const std::string &_accept);

  void findHeader(std::string &key, std::istringstream &isLine);
  void sendResponse(int clientFD, std::string responseBody);
  std::string createResponseBody(void);
  std::string extractHtmlContent(const std::string &filePath);
};

#endif