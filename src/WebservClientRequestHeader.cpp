/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   WebservClientRequestHeader.cpp                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: demre <demre@student.42malaga.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/01 20:07:09 by demre             #+#    #+#             */
/*   Updated: 2024/08/10 12:24:41 by demre            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Webserv.hpp"
#include "core.hpp"

void Webserv::parseClientRequest(ClientRequest &req)
{
  std::istringstream iss(req.buffer);
  std::string line;

  // Parse the first line (request line)
  if (std::getline(iss, line))
  {
    std::istringstream lineStream(line);
    lineStream >> req.method;
    lineStream >> req.URI;
    lineStream >> req.HTTPversion;

    if ((req.method != "GET" && req.method != "POST" && req.method != "DELETE")
        /* || req.HTTPversion != "HTTP/1.1" */)
    {
      req.buffer.erase();
      throw HttpException(400, "Bad request");
    }
  }
  else
  {
    req.buffer.erase();
    throw HttpException(400, "Bad request");
  }

  // Parse the remaining header fields
  while (std::getline(iss, line))
  {
    if (line.empty() || line == "\r")
      break; // Stop parsing on an empty line: end of headers

    std::string::size_type separator = line.find(':');
    if (separator != std::string::npos)
    {
      std::string fieldName = line.substr(0, separator);
      std::string fieldValue = line.substr(separator + 1);

      // Trim any leading whitespace from the field value
      fieldValue.erase(0, fieldValue.find_first_not_of(" \t"));

      req.fields[fieldName] = fieldValue;
    }
    else
    {
      req.buffer.erase();
      throw HttpException(400, "Bad request: Malformed header line");
    }
  }
}