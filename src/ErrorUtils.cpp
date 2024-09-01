/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ErrorUtils.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: demre <demre@student.42malaga.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/01 19:54:52 by demre             #+#    #+#             */
/*   Updated: 2024/09/01 15:12:56 by demre            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ErrorUtils.hpp"
#include "HttpExceptions.hpp"

std::vector<char> composeErrorHtmlPage(int const &statusCode,
                                       std::string const &errorMessage,
                                       std::map<int, std::string> errorPages)
{
  std::string errorBody;
  std::vector<char> charVecResponse;

  // if code found and corresponding file accessible (correct path)
  if (errorPages.find(statusCode) != errorPages.end()
      && isFile(errorPages[statusCode]))
    errorBody = extractHtmlContentFromFile(errorPages[statusCode]);
  else //
    errorBody = "<html><body><h1>" + toString(statusCode) + " " + errorMessage
                + "</h1></body></html>\r\n\r\n";

  std::ostringstream response;
  response << "HTTP/1.1 " << statusCode << " " << errorMessage + "\r\n"
           << "Date: " << getHttpDate() << "\r\n"
           << "Content-Type: text/html\r\n"
           << "Content-Length: " << errorBody.size() << "\r\n"
           << "Connection: close\r\n"
           << "\r\n"
           << errorBody;

  std::string responseStr = response.str();
  charVecResponse.insert(charVecResponse.begin(), responseStr.begin(),
                         responseStr.end());
  return (charVecResponse);
}
