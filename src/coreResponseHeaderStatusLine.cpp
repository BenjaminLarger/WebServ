/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coreResponseHeaderStatusLine.cpp                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: demre <demre@student.42malaga.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/07 16:02:03 by demre             #+#    #+#             */
/*   Updated: 2024/08/07 16:51:50 by demre            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "core.hpp"

// Function to initialize the map
std::map<int, std::string> createStatusCodeMap()
{
  // Map of HTTP status codes to their reason phrases
  std::map<int, std::string> m;
  m[100] = "Continue";
  m[101] = "Switching Protocols";
  m[200] = "OK";
  m[201] = "Created";
  m[202] = "Accepted";
  m[203] = "Non-Authoritative Information";
  m[204] = "No Content";
  m[205] = "Reset Content";
  m[206] = "Partial Content";
  m[207] = "Multi-Status ";
  m[208] = "Already Reported";
  m[226] = "IM Used";
  m[300] = "Multiple Choices";
  m[301] = "Moved Permanently";
  m[302] = "Found";
  m[303] = "See Other";
  m[304] = "Not Modified";
  m[305] = "Use Proxy";
  m[306] = "unused";
  m[307] = "Temporary Redirect";
  m[308] = "Permanent Redirect";
  m[400] = "Bad Request";
  m[401] = "Unauthorized";
  m[402] = "Payment Required";
  m[403] = "Forbidden";
  m[404] = "Not Found";
  m[405] = "Method Not Allowed";
  m[406] = "Not Acceptable";
  m[407] = "Proxy Authentication Required";
  m[408] = "Request Timeout";
  m[409] = "Conflict";
  m[410] = "Gone";
  m[411] = "Length Required";
  m[412] = "Precondition Failed";
  m[413] = "Request Entity Too Large";
  m[414] = "Request-URI Too Long";
  m[415] = "Unsupported Media Type";
  m[416] = "Requested Range Not Satisfiable";
  m[417] = "Expectation Failed";
  m[421] = "Misdirected Request";
  m[421] = "Locked";
  m[429] = "Too Many Requests";
  m[431] = "Request Header Fields Too Large";
  m[451] = "Unavailable For Legal Reasons";
  m[500] = "Internal Server Error";
  m[501] = "Not Implemented";
  m[502] = "Bad Gateway";
  m[503] = "Service Unavailable";
  m[504] = "Gateway Timeout";
  m[505] = "HTTP Version Not Supported";
  return (m);
}

std::string getReasonPhrase(const int &code)
{
  static const std::map<int, std::string> statusCodeMap = createStatusCodeMap();

  std::map<int, std::string>::const_iterator it = statusCodeMap.find(code);

  if (it != statusCodeMap.end())
    return (it->second);
  else
    return ("Unknown Status Code");
}

std::string getHeaderStatusLine(const int &code)
{
  // ex: "HTTP/1.1 301 Moved Permanently\r\n";
  std::ostringstream response;
  response << "HTTP/1.1 " << code << " " << getReasonPhrase(code) << "\r\n";

  return (response.str());
}