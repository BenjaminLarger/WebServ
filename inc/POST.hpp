/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   POST.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: demre <demre@student.42malaga.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/01 19:56:16 by demre             #+#    #+#             */
/*   Updated: 2024/08/01 19:56:16 by demre            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#ifndef POST_HPP
#define POST_HPP

#include "ErrorUtils.hpp"
#include "Webserv.hpp"
#include "utils.hpp"

class POST
{

private:
  // Request line
  std::map<std::string, std::string> headers;
  std::istringstream requestStream;
  std::string pathToRessource;
  std::string HTTPversion;
  // Header
  std::string host;
  std::string contentType;
  int contentLength;
  // Body
  std::string body;

  void extractFirstLine();
  void extractHeaders();
  void extractBody(int clientFD);

public:
  POST(int serverFD, int clientFD, std::string &clientInput);
  POST();
  ~POST(void);
};

#endif