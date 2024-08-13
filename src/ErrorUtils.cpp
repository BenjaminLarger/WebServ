/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ErrorUtils.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: demre <demre@student.42malaga.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/01 19:54:52 by demre             #+#    #+#             */
/*   Updated: 2024/08/13 21:56:51 by demre            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ErrorUtils.hpp"
#include "HttpExceptions.hpp"

// void sendErrorResponse(int clientSocket, int statusCode,
//                        const std::string &statusMessage, std::string errorBody)
// {
//   std::ostringstream response;
//   response << "HTTP/1.1 " << statusCode << " " << statusMessage + "\r\n"
//            << "Content-Type: text/html\r\n"
//            << "Content-Length: " << errorBody.size() << "\r\n"
//            << "Connection: close\r\n\r\n"
//            << errorBody;

//   std::string responseStr = response.str();
//   if (sendall(clientSocket, responseStr.c_str(), responseStr.size()) == -1)
//   {
//     throw HttpException(500, "Internal Server Error: Data failed to be sent to "
//                              "the client (sendall)");
//   }
// }

std::string sendDefaultErrorPage(int const &statusCode,
                                 std::string const &errorMessage,
                                 std::map<int, std::string> errorPages)
{
  std::string errorBody;
  // if code found and file accessible (correct path)
  if (errorPages.find(statusCode) != errorPages.end()
      && isFile(errorPages[statusCode]))
    errorBody = extractHtmlContentFromFile(errorPages[statusCode]);
  else //
    errorBody = "<html><body><h1>" + toString(statusCode) + " " + errorMessage
                + "</h1></body></html>";

  std::ostringstream response;
  response << "HTTP/1.1 " << statusCode << " " << errorMessage + "\r\n"
           << "Content-Type: text/html\r\n"
           << "Content-Length: " << errorBody.size() << "\r\n"
           << "Connection: close\r\n\r\n"
           << errorBody;

  return (response.str());
}
