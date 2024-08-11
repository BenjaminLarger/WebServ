/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   DELETE.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blarger <blarger@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/07 15:57:51 by isporras          #+#    #+#             */
/*   Updated: 2024/08/11 17:22:12 by blarger          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
	std::string	handleLocations(std::string pathToResource);
	std::string replaceLocationByRoot(std::string pathToResource
							, std::string oldPrefix, std::string newPrefix);
};
