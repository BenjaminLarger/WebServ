/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ErrorUtils.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: demre <demre@student.42malaga.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/01 19:54:52 by demre             #+#    #+#             */
/*   Updated: 2024/08/07 15:58:30 by demre            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ErrorUtils.hpp"
#include "HttpExceptions.hpp"

void sendErrorResponse(int clientSocket, std::string statusCode,
                       const std::string &statusMessage, std::string errorBody)
{
  std::ostringstream response;
  response << "HTTP/1.1 " << statusCode << " " << statusMessage + "\r\n";
  response << "Content-Type: text/html\r\n";
  response << "Content-Length: " << errorBody.size() << "\r\n";
  response << "Connection: close\r\n\r\n";
  response << errorBody;

  std::string responseStr = response.str();
  if (sendall(clientSocket, responseStr.c_str(), responseStr.size()) == -1)
    throw HttpException(
        "500", "Internal Server Error: Data failed to be sent to the client");
}

void sendDefaultErrorPage(int clientSocket, std::string statusCode,
                          std::string errorMessage,
                          std::map<int, std::string> errorPages)
{
  std::string response;

  if (errorPages.find(std::atoi(statusCode.c_str())) == errorPages.end())
    response = "<html><body><h1>" + statusCode + " " + errorMessage
               + "</h1></body></html>";
  else
    response = extractHtmlContentFromFile(
        "var/www/errors" + errorPages[std::atoi(statusCode.c_str())]);
  try
  {
    sendErrorResponse(clientSocket, statusCode, errorMessage, response);
  }
  catch (const HttpException &e)
  {
    std::cerr << RED << "Error: " << e.what() << RESET << '\n';
  }
}
