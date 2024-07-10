/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   GET.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blarger <blarger@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/09 18:24:19 by demre             #+#    #+#             */
/*   Updated: 2024/07/10 13:00:52 by blarger          ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#pragma once

#ifndef GET_HPP
#define GET_HPP

#include "Webserv.hpp"

class GET : public Webserv
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
  GET(Webserv _server, int serverFD, int clientFD, std::string &clientInput);
  GET();
  ~GET(void);

  void findHeader(std::string &key, std::istringstream &isLine);
  void	sendResponse(int clientFD);
  std::string	createResponseBody(void);
  std::string extractHtmlContent(const std::string& filePath);
};

#endif