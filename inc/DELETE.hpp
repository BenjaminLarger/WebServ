/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   DELETE.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: demre <demre@student.42malaga.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/07 15:57:51 by isporras          #+#    #+#             */
/*   Updated: 2024/08/08 16:23:15 by demre            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DELETE_HPP
#define DELETE_HPP

#include "ErrorUtils.hpp"
#include "HttpExceptions.hpp"
#include "Webserv.hpp"

class DELETE
{
private:
  // Request line
  std::string pathToRessource;
  std::string HTTPversion;
  // Header
  std::string host;
  std::string authorization;
  std::string if_match;
  // ServerConfig
  const ServerConfig &serverConfig;

public:
  DELETE(ClientInfo &client, int clientFD, std::string &clientInput,
         const ServerConfig &serverConfig);
  ~DELETE(void);

  void setHost(const std::string &_host);
  void setAuthorization(const std::string &_authorization);
  void setIfMatch(const std::string &_if_match);
  std::string getHost(void) const;
  std::string getAuthorization(void) const;
  std::string getIfMatch(void) const;

  void parseRequest(std::string &clientInput);
  void findHeader(std::istringstream &isLine);
  void checkPreconditions(std::string expectedEtag, std::string expectedAuth);
};
#endif