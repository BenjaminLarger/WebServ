/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   DELETE.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blarger <blarger@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/07 15:57:35 by isporras          #+#    #+#             */
/*   Updated: 2024/09/02 13:21:58 by blarger          ###   ########.fr       */
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