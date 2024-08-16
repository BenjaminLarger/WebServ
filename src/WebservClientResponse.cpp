/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   WebservClientResponse.cpp                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: demre <demre@student.42malaga.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/01 20:07:09 by demre             #+#    #+#             */
/*   Updated: 2024/08/16 15:19:46 by demre            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "DELETE.hpp"
#include "GET.hpp"
#include "POST.hpp"
#include "Webserv.hpp"
#include "core.hpp"

void Webserv::handleClientResponse(size_t &i)
{
  try
  {
    if (clients[i].totalBytesSent < clients[i].totalToSend)
    {
      // size_t bytesToSend
      //     = std::min(250, clients[i].totalToSend - clients[i].totalBytesSent);
      int bytesSent
          = send(clients[i].socketFD,
                 clients[i].response.c_str() + clients[i].totalBytesSent,
                 clients[i].totalToSend - clients[i].totalBytesSent,
                 /* bytesToSend, */
                 0);
      if (bytesSent == -1)
      {
        std::cout << "bytesSent == -1 " << std::endl;
        closeConnection(i);
        i--;
        throw HttpException(500, strerror(errno));
      }
      else if (bytesSent == 0)
      {
        std::cout << "bytesSent == 0 " << bytesSent << std::endl;
        clients[i].totalBytesSent = 0;
        clients[i].totalToSend = 0;
        clients[i].response.clear();
      }
      else if (bytesSent > 0)
      {
        std::cout << "Bytes sent: " << bytesSent << std::endl;
        clients[i].totalBytesSent += bytesSent;
      }
    }
    else
    {
      std::cout << "(totalBytesSent >= totalToSend) "
                << clients[i].totalBytesSent << " " << clients[i].totalToSend
                << std::endl;
      clients[i].totalBytesSent = 0;
      clients[i].totalToSend = 0;
      clients[i].response.clear();
    }
  }
  catch (const HttpException &e)
  {
    std::cerr << RED << "Error: " << e.getStatusCode() << " " << e.what()
              << RESET << '\n';

    clients[i].response = composeErrorHtmlPage(
        e.getStatusCode(), getReasonPhrase(e.getStatusCode()),
        clients[i].client_serverConfig.errorPages);
  }
}
