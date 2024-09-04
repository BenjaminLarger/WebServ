/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   WebservClientRequestHeader.cpp                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: demre <demre@student.42malaga.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/01 20:07:09 by demre             #+#    #+#             */
/*   Updated: 2024/09/04 15:13:44 by demre            ###   ########.fr       */
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
  req.bodyTooLarge = false;
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
}

void Webserv::checkBodySize(ClientRequest &req, long long int maxBodySize,
                            size_t &i)
{
  long long int bodyLength
      = std::strtol(req.fields["Content-Length"].c_str(), NULL, 10);
  if (maxBodySize > 0 && bodyLength > maxBodySize)
  {
    fds[i].events &= ~POLLIN;
    fds[i].events |= POLLOUT;
    req.bodyTooLarge = true;
    throw(HttpException(413, "Payload too large"));
  }
}

void Webserv::checkBodySize(ClientRequest &req, size_t &i,
                            long long int maxBodySize)
{
  long long int contentLength
      = std::strtol(req.fields["Content-Length"].c_str(), NULL, 10);
  // Convert bodyLength and maxBodySize from bytes to megabytes
  double contentLengthKB = contentLength / (1024.0);
  double bodySizeKB = req.buffer.size() / (1024.0);
  double maxBodySizeKB = maxBodySize / (1024.0);

  // Compare the body size to the max body size
  if (maxBodySize > 0 && contentLength > maxBodySize)
  {
    fds[i].events &= ~POLLIN;
    fds[i].events |= POLLOUT;
    req.bodyTooLarge = true;
    std::cout << ORANGE << "Body size " << contentLengthKB
              << " kB exceeds the maximum allowed size " << maxBodySizeKB
              << " kB" << RESET << std::endl;
  }
  else
  {
    std::cout << YELLOW << "Bytes received = " << bodySizeKB << " kB" << RESET
              << std::endl;
  }
}

void Webserv::parseClientRequest(ClientRequest &req, long long int maxBodySize,
                                 size_t &i, bool &isError)
{
  reqReset(req);
  std::istringstream iss(req.buffer);
  std::string line;

  // Parse the first line (request line)
  if (std::getline(iss, line))
  {
    std::istringstream lineStream(line);
    lineStream >> req.method;
    lineStream >> req.URIpath;
    lineStream >> req.HTTPversion;
    if ((req.method != "GET" && req.method != "POST" && req.method != "DELETE")
        || req.HTTPversion != "HTTP/1.1")
      isError = true;
    req.URIpath = urlDecode(req.URIpath);
    extractQueryString(req);
  }
  else
    isError = true;

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
      isError = true;
      break;
    }
  }
  checkBodySize(req, i, maxBodySize);
}