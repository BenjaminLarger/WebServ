/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerConfig.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: demre <demre@student.42malaga.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/31 18:15:10 by demre             #+#    #+#             */
/*   Updated: 2024/08/04 17:43:49 by demre            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* HTML5 references

There are three possibilities for enctype:
- application/x-www-form-urlencoded
- multipart/form-data (spec points to RFC7578)
- text/plain. This is "not reliably interpretable by computer", so it should never be used in production, and we will not look further into it. */

#include "ServerConfig.hpp"
#include "utils.hpp"

ServerConfig::ServerConfig(void) {}

ServerConfig::~ServerConfig(void) {}

/* --------------GETTER */

std::string ServerConfig::getServerNames(void) const
{
  std::cout << ".size(" << serverNames.size() << "): ";

  std::string serverNamesStr;
  for (size_t i = 0; i < this->serverNames.size(); i++)
  {
    if (i > 0)
    {
      serverNamesStr += ' ';
    }
    serverNamesStr += this->serverNames[i];
  }
  return (serverNamesStr);
}

const std::string &ServerConfig::getServerName(int index) const
{
  return (this->serverNames[index]);
}

const int &ServerConfig::getPort() const { return (this->port); }

const std::string &ServerConfig::getHost(void) const { return (this->host); }

/* --------------SETTER */

void ServerConfig::addServerName(std::string newServerName)
{
  serverNames.push_back(newServerName);
}

void ServerConfig::addPort(int newPort) { this->port = newPort; }

/* --------------MEMBER FUNCTIONS */
