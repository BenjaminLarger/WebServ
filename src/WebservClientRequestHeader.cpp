/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   WebservClientRequestHeader.cpp                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blarger <blarger@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/01 20:07:09 by demre             #+#    #+#             */
/*   Updated: 2024/08/10 20:49:40 by blarger          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Webserv.hpp"
#include "core.hpp"

void Webserv::parseClientRequest(ClientRequest &req)
{
  std::istringstream iss(client.req.buffer);
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
      client.req.buffer.erase();
      throw HttpException(400, "Bad request: Method not implemented.");
    }
  }
  else
  {
    client.req.buffer.erase();
    throw HttpException(400, "Bad request: There is no first line in header.");
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
      client.req.buffer.erase();
      throw HttpException(400, "Bad request: Malformed header line");
    }
  }
}