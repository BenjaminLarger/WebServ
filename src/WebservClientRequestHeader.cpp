/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   WebservClientRequestHeader.cpp                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blarger <blarger@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/01 20:07:09 by demre             #+#    #+#             */
/*   Updated: 2024/08/29 15:52:26 by blarger          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Webserv.hpp"
#include "core.hpp"

// Clears the struct ClientRequest before parsing new request
static void reqReset(ClientRequest &req)
{
  req.method.erase();
  req.URIpath.erase();
  req.queryString.erase();
  req.HTTPversion.erase();
  req.fields.clear();
  req.pathFolder.erase();
  req.pathOnServer.erase();
  req.pathFolderOnServer.erase();
}

// Modifies req.URIpath to exclude the query string and stores the query string separately in req.queryString.
static void extractQueryString(ClientRequest &req)
{
  std::string::size_type lastSlashPos = req.URIpath.find_last_of('/');
  if (lastSlashPos == std::string::npos)
    return;

  std::string::size_type queryPos = req.URIpath.find('?', lastSlashPos);

  // If '?' is found, separate the query string from the URI
  if (queryPos != std::string::npos)
  {
    req.queryString = req.URIpath.substr(queryPos + 1);
    req.URIpath = req.URIpath.substr(0, queryPos);
  }
  else
    req.queryString.erase();

  // std::cout << "extractQueryString URIpath: " << req.URIpath
  //           << ", req.queryString: " << req.queryString << std::endl;
}

void Webserv::parseClientRequest(ClientRequest &req, long long int maxBodySize, size_t &i)
{
  reqReset(req);

  std::istringstream iss(req.buffer);
  std::string line;

  /* std::cout << GREEN << "Client request buffer: " << std::endl
            << req.buffer << RESET << std::endl; */
  // Parse the first line (request line)
  if (std::getline(iss, line))
  {
    std::istringstream lineStream(line);
    lineStream >> req.method;
    lineStream >> req.URIpath;
    lineStream >> req.HTTPversion;
    if ((req.method != "GET" && req.method != "POST" && req.method != "DELETE")
        /* || req.HTTPversion != "HTTP/1.1" */)
    {
			std::cout << req.method << std::endl;
      req.buffer.erase();
      throw HttpException(400, "Bad request: Method not implemented.");
    }
    extractQueryString(req);
  }
  else
  {
		std::cout << "response : " << req.buffer << std::endl;
    req.buffer.erase();
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

      trimTrailingWS(fieldName);
      trimTrailingWS(fieldValue);
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
	long long int	bodyLength = std::strtol(req.fields["Content-Length"].c_str(), NULL, 10);
	std::cout << RED << "bodyLength = " << bodyLength << ", maxBodySize = " << (maxBodySize * 1024 * 1024) << std::endl;
	if (maxBodySize > 0 && bodyLength > (maxBodySize * 1024 * 1024))
	{
		(void)i;
		/* closeConnection(i);
       --i; */
		/* close(fds[i].fd);
  	fds.erase(fds.begin() + i);
	  clients.erase(clients.begin() + i); */
		throw (HttpException(413, "Payload too large"));
	}
}