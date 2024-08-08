/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coreResponse.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: demre <demre@student.42malaga.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/07 15:49:50 by demre             #+#    #+#             */
/*   Updated: 2024/08/08 14:08:32 by demre            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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

std::string composeOkHtmlResponse(std::string responseBody)
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

std::string createRedirectResponse(const int &code, const std::string &location)
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
        500, "Internal Server Error: Data failed to be sent to the client");
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
  if (!file.read(&buffer[0], size))
  {
    std::cerr << "Failed to read file" << std::endl;
    buffer.clear();
    return (buffer);
  }

  return (buffer);
}

std::string composeFileResponse(const std::vector<char> &fileContent,
                                std::string filepath)
{
  std::string response;
  response += "HTTP/1.1 200 OK\r\n";
  response += "Content-Type: " + getMediaType(filepath) + "\r\n";
  response += "Content-Length: " + toString(fileContent.size()) + "\r\n";
  response += "Cache-Control: no-cache\r\n";
  response += "\r\n";

  // Body (binary data)
  response.append(&fileContent[0], fileContent.size());

  return (response);
}
