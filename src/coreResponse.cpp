/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coreResponse.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blarger <blarger@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/07 15:49:50 by demre             #+#    #+#             */
/*   Updated: 2024/09/03 18:07:09 by blarger          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Cookies.hpp"
#include "core.hpp"

std::string extractHtmlContentFromFile(const std::string &filePath)
{
  std::ifstream file(filePath.c_str());
  if (!file.is_open())
    throw HttpException(404, "Not Found " + filePath);

  std::stringstream buffer;
  buffer << file.rdbuf();
  buffer << "\r\n";
  return (buffer.str());
}

std::vector<char> composeOkHtmlResponse(
    std::string responseBody, ClientRequest &clientReq)
{
  std::ostringstream response;
  std::vector<char> charVecResponse;

  std::string sessionId = getCookieRequestLine(clientReq);

  response << "HTTP/1.1 200 OK\r\n"
           << "Date: " << getHttpDate() << "\r\n"
           << "Content-Type: text/html\r\n"
           << "Content-Length: " << responseBody.size() << "\r\n"
           << "Cache-Control: no-cache\r\n"
           << sessionId << "\r\n"
           << "\r\n"
           << responseBody;

  std::string responseStr = response.str();
  charVecResponse.insert(charVecResponse.begin(), responseStr.begin(),
                         responseStr.end());
  return (charVecResponse);
}

std::vector<char> composeDeleteOkHtmlResponse(ClientRequest &clientReq)
{
  std::ostringstream response;
  std::vector<char> charVecResponse;
  std::string sessionId = getCookieRequestLine(clientReq);

  response << "HTTP/1.1 204 No Content\r\n"
           << "Date: " << getHttpDate() << "\r\n"
           << "Content-Length: 0\r\n"
           << sessionId << "Connection: close\r\n"
           << "\r\n";

  std::string responseStr = response.str();
  charVecResponse.insert(charVecResponse.begin(), responseStr.begin(),
                         responseStr.end());
  return (charVecResponse);
}

std::vector<char> createRedirectResponse(
    const int &code, const std::string &location, ClientRequest &clientReq)
{
  std::vector<char> charVecResponse;
  std::ostringstream response;
  std::string sessionId = getCookieRequestLine(clientReq);

  response << getHeaderStatusLine(code) << "Location: " << location << "\r\n"
           << "Date: " << getHttpDate() << "\r\n"
           << "Content-Length: 0\r\n"
           << sessionId << "Cache-Control: no-cache\r\n"
           << "\r\n";

  std::string responseStr = response.str();
  charVecResponse.insert(charVecResponse.begin(), responseStr.begin(),
                         responseStr.end());
  return (charVecResponse);
}

std::vector<char> readFile(const std::string &filename)
{
  std::ifstream file(filename.c_str(), std::ios::binary | std::ios::ate);
  if (!file)
  {
    std::cerr << "Failed to open file" << std::endl;
    std::vector<char> buffer;
    return (buffer);
  }

  std::streamsize size = file.tellg();
  file.seekg(0, std::ios::beg);

  std::vector<char> buffer(size);
  if (!file.read(buffer.data(), size))
  {
    std::cerr << "Failed to read file" << std::endl;
    buffer.clear();
    return (buffer);
  }

  return (buffer);
}

std::vector<char> composeFileResponse(
    const std::vector<char> &fileContent, std::string filepath,
    ClientRequest &clientReq)
{
  std::string response;
  std::vector<char> charVecResponse;
  std::string sessionId = getCookieRequestLine(clientReq);

  response += "HTTP/1.1 200 OK\r\n";
  response += "Date: " + getHttpDate() + "\r\n";
  response += "Content-Type: " + getMediaType(filepath) + "\r\n";
  response += "Content-Length: " + toString(fileContent.size()) + "\r\n";
  response += sessionId;
  response += "Cache-Control: no-cache\r\n";
  response += "\r\n";

  // Body (binary data)
  response.append(&fileContent[0], fileContent.size());
  charVecResponse.insert(charVecResponse.begin(), response.begin(),
                         response.end());
  charVecResponse.insert(charVecResponse.end(), fileContent.begin(),
                         fileContent.end());

  return (charVecResponse);
}
