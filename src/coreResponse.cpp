/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coreResponse.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: demre <demre@student.42malaga.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/07 15:49:50 by demre             #+#    #+#             */
/*   Updated: 2024/08/30 16:07:08 by demre            ###   ########.fr       */
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

std::vector<char> composeOkHtmlResponse(std::string responseBody,
                                        std::string reqBuffer)
{
  std::ostringstream response;
  std::vector<char> charVecResponse;

  std::string sessionId;
  size_t findSessionId = reqBuffer.find("sessionId");
  if (findSessionId == std::string::npos)
  {
    std::cout << GREEN
              << "session ID not found in request => generating new one !\n"
              << RESET << std::endl;
    sessionId
        = "Set-Cookie: sessionId=" + generateSessionID() + "; HttpOnly\r\n";
  }
  else
  {
    std::cout << RED << "Session ID not found\n" << RESET << std::endl;
    sessionId = "Set-Cookie: sessionId=" + findSessionID(reqBuffer)
                + "; HttpOnly\r\n";
  }

  response << "HTTP/1.1 200 OK\r\n"
           << "Date: " << getHttpDate() << "\r\n"
           << "Content-Type: text/html\r\n"
           << "Content-Length: " << responseBody.size() << "\r\n"
           << "Cache-Control: no-cache\r\n"
           << sessionId                        //Testing cookies
           << "Set-Cookie: theme=light\r\n"    //Testing cookies
           << "Set-Cookie: rememberMe=yes\r\n" //Testing cookies
           << "\r\n"
           << responseBody;

  std::string responseStr = response.str();
  std::cout << YELLOW << "before charVecResponse" << RESET << std::endl;
  charVecResponse.insert(charVecResponse.begin(), responseStr.begin(),
                         responseStr.end());

  std::cout << YELLOW << "after charVecResponse" << RESET << std::endl;
  return (charVecResponse);
}

std::vector<char> composeDeleteOkHtmlResponse()
{
  std::ostringstream response;
  std::vector<char> charVecResponse;

  response << "HTTP/1.1 204 No Content\r\n"
           << "Date: " << getHttpDate() << "\r\n"
           << "Content-Length: 0\r\n"
           << "Connection: close\r\n\r\n";

  std::string responseStr = response.str();
  charVecResponse.insert(charVecResponse.begin(), responseStr.begin(),
                         responseStr.end());
  return (charVecResponse);
}

std::vector<char> createRedirectResponse(const int &code,
                                         const std::string &location)
{
  // std::cout << BLUE << "Building redirection header : " << location
  //           << std::endl;
  std::vector<char> charVecResponse;
  std::ostringstream response;
  response << getHeaderStatusLine(code) << "Location: " << location << "\r\n"
           << "Date: " << getHttpDate() << "\r\n"
           << "Content-Length: 0\r\n"
           << "Cache-Control: no-cache\r\n"
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
/* 
void	uploadFile(const std::vector<char> &fileContent,
                                std::string filepath)
{
	std::string	filePath = "./var/www/uploads/files/test.mp4";
	std::cout << GREEN << "filepath = " << filePath << RESET << std::endl;
	std::ofstream outFile(filePath.c_str(), std::ios::binary);
			if (!outFile)
				throw HttpException(400, strerror(errno));
	
} */

std::vector<char> composeFileResponse(const std::vector<char> &fileContent,
                                      std::string filepath)
{
  std::string response;
  std::vector<char> charVecResponse;
  response += "HTTP/1.1 200 OK\r\n";
  response += "Date: " + getHttpDate() + "\r\n";
  response += "Content-Type: " + getMediaType(filepath) + "\r\n";
  response += "Content-Length: " + toString(fileContent.size()) + "\r\n";
  response += "Cache-Control: no-cache\r\n";
  response += "\r\n";

  // Body (binary data)

  //std::cout << YELLOW << "response : " << response << RESET << std::endl;
  response.append(&fileContent[0], fileContent.size());
  charVecResponse.insert(charVecResponse.begin(), response.begin(),
                         response.end());
  charVecResponse.insert(charVecResponse.end(), fileContent.begin(),
                         fileContent.end());
  //uploadFile(filepath, fileContent);
  return (charVecResponse);
}
