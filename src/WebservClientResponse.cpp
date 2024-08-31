/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   WebservClientResponse.cpp                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blarger <blarger@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/01 20:07:09 by demre             #+#    #+#             */
/*   Updated: 2024/08/30 16:17:25 by blarger          ###   ########.fr       */
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
			//std::cout << GREEN << "response to client : " << clients[i].response << RESET << std::endl;
			//std::cout << "clients[i].totalBytesSent = " << clients[i].totalBytesSent << ", clients[i].totalToSend = " << clients[i].totalToSend << std::endl;
      int bytesSent
          = send(clients[i].socketFD,
                 clients[i].response.data() + clients[i].totalBytesSent,
                 clients[i].totalToSend - clients[i].totalBytesSent,
                 0);
      if (bytesSent == -1)
      {
        std::cout << "bytesSent == -1 " << std::endl;
        closeConnection(i);
        i--;
        throw HttpException(500, "Internal Server Error: Failed to send response.");
      }
      else if (bytesSent == 0)
      {
        std::cout << "bytesSent == 0 " << bytesSent << std::endl;
        clients[i].totalBytesSent = 0;
        clients[i].totalToSend = 0;
        clients[i].response.clear();
        if (clients[i].req.bodyTooLarge == true ||
					clients[i].req.shouldCloseConnection == true)
        {
          closeConnection(i);
          --i;
        }
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
      if (clients[i].req.bodyTooLarge == true
				|| clients[i].req.shouldCloseConnection == true)
      {
				std::cout << RED << "shouldCloseConnection == true" << RESET << std::endl;
        closeConnection(i);
        --i;
      }
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
