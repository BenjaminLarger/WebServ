/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ErrorUtils.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isporras <isporras@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/01 19:54:52 by demre             #+#    #+#             */
/*   Updated: 2024/08/06 19:25:47 by isporras         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ErrorUtils.hpp"

void sendErrorResponse(int clientSocket, std::string statusCode,
                       const std::string &statusMessage, std::string errorBody)
{
  std::ostringstream  response;
      response << "HTTP/1.1 " << statusCode << " " << statusMessage + "\r\n";
  response << "Content-Type: text/html\r\n\r\n";
  response << errorBody;

  std::string responseStr = response.str();
  if (sendall(clientSocket, responseStr.c_str(), responseStr.size()) == -1)
    perror("Data failed to be sent to the client");
}

void sendDefaultErrorPage(int clientSocket, std::string statusCode, std::string errorMessage,  std::map<int, std::string> errorPages)
{
  std::string response;
  
  if (errorPages.find(std::atoi(statusCode.c_str())) == errorPages.end())
    response = "HTTP/1.1 " + statusCode + " " + errorMessage + "\r\n";
  else
    response = extractHtmlContent("var/www/errors" + errorPages[std::atoi(statusCode.c_str())]);
  sendErrorResponse(clientSocket, statusCode, errorMessage, response);
}


