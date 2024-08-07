/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coreResponse.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: demre <demre@student.42malaga.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/07 15:49:50 by demre             #+#    #+#             */
/*   Updated: 2024/08/07 16:33:06 by demre            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "core.hpp"

std::string extractHtmlContentFromFile(const std::string &filePath)
{
  std::ifstream file(filePath.c_str());
  if (!file.is_open())
    throw std::runtime_error("Could not open file: " + filePath);

  std::stringstream buffer;
  buffer << file.rdbuf();
  buffer << "\r\n";

  return (buffer.str());
}

// Return a HTML response with the given body
std::string addOkResponseHeaderToBody(std::string responseBody)
{
  std::ostringstream response;
  response << "HTTP/1.1 200 OK\r\n"
           << "Content-Type: text/html\r\n"
           << "Content-Length: " << responseBody.size() << "\r\n"
           << "Cache-Control: no-cache\r\n"
           << "\r\n"
           << responseBody;

  return (response.str());
}

std::string redirectionHeader(const int &code, const std::string &location)
{
  std::cout << BLUE << "Building redirection header : " << location
            << std::endl;

  std::ostringstream response;
  response << getHeaderStatusLine(code) << "Location: " << location << "\r\n"
           << "Content-Length: 0\r\n"
           << "Cache-Control: no-cache\r\n"
           << "\r\n";

  return (response.str());
}

void sendRGeneric(int clientFD, std::string responseStr)
{
  if (sendall(clientFD, responseStr.c_str(), responseStr.size()) == -1)
    throw HttpException(
        "500", "Internal Server Error: Data failed to be sent to the client");
}
