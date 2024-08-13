/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ErrorUtils.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blarger <blarger@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/01 19:54:52 by demre             #+#    #+#             */
/*   Updated: 2024/08/13 17:00:56 by blarger          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ErrorUtils.hpp"
#include "HttpExceptions.hpp"

std::string sendErrorResponse(int clientSocket, int statusCode,
                       const std::string &statusMessage, std::string errorBody)
{
	(void)clientSocket;
  std::ostringstream response;
  response << "HTTP/1.1 " << statusCode << " " << statusMessage + "\r\n"
           << "Content-Type: text/html\r\n"
           << "Content-Length: " << errorBody.size() << "\r\n"
           << "Connection: close\r\n\r\n"
           << errorBody;

	return (response.str());
  /* if (sendall(clientSocket, responseStr.c_str(), responseStr.size()) == -1)
  {
    throw HttpException(
        500, "Internal Server Error: Data failed to be sent to the client (sendall)");
  } */
}

std::string sendDefaultErrorPage(int clientSocket, int statusCode,
                          std::string errorMessage,
                          std::map<int, std::string> errorPages)
{
  std::string response;

  // if code found
  if (errorPages.find(statusCode) != errorPages.end())
    response
        = extractHtmlContentFromFile("var/www/errors" + errorPages[statusCode]);
  else // 
    response = "<html><body><h1>" + toString(statusCode) + " " + errorMessage
               + "</h1></body></html>";
	(void)clientSocket;
	return (sendErrorResponse(clientSocket, statusCode, errorMessage, response));	
}
