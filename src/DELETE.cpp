/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   DELETE.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: demre <demre@student.42malaga.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/07 15:57:35 by isporras          #+#    #+#             */
/*   Updated: 2024/08/16 10:01:08 by demre            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "DELETE.hpp"
#include "HttpExceptions.hpp"
#include "core.hpp"

DELETE::DELETE(ClientInfo &client, const ServerConfig &serverConfig)
{
  try
  {
    std::string pathOnServer = client.req.pathOnServer;
    // std::cout << "pathOnServer:: " << pathOnServer << ", isFile(pathOnServer) "
    //           << isFile(pathOnServer) << std::endl;

    if (!isFile(pathOnServer))
      throw HttpException(404, "File not found");

    if (remove(pathOnServer.c_str()) != 0)
      throw HttpException(500, "Internal Server Error");
    else
    {
      client.response = composeDeleteOkHtmlResponse();
      client.totalToSend = client.response.size();
      client.totalBytesSent = 0;
    }

    std::cout << GREEN
              << "Deleted file and response 204 No Content sent: " << RESET
              << pathOnServer << std::endl;
  }
  catch (const HttpException &e)
  {
    std::cerr << RED << "Error: " << e.getStatusCode() << " " << e.what()
              << RESET << '\n';

    client.response = composeErrorHtmlPage(e.getStatusCode(),
                                           getReasonPhrase(e.getStatusCode()),
                                           serverConfig.errorPages);
    client.totalToSend = client.response.size();
    client.totalBytesSent = 0;
  }
}

DELETE::~DELETE() {}