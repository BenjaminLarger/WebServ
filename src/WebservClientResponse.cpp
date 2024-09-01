/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   WebservClientResponse.cpp                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: demre <demre@student.42malaga.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/01 20:07:09 by demre             #+#    #+#             */
/*   Updated: 2024/09/01 15:32:03 by demre            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "DELETE.hpp"
#include "GET.hpp"
#include "POST.hpp"
#include "Webserv.hpp"
#include "core.hpp"

bool Webserv::checkCloseConnectionReq(ClientRequest &req)
{
  if (req.fields["Connection"] == "close")
    return (true);
  return (false);
}

bool Webserv::checkCloseConnectionResp(std::vector<char> response)
{
  // Check if the header of the response contains "Connection: close"
  std::string responseStr(response.begin(), response.end());
  size_t bodyPos = responseStr.find("\r\n\r\n");
  std::string headers = responseStr.substr(0, bodyPos);

  if (headers.find("Connection: close") != std::string::npos)
    return (true);
  return (false);
}

void Webserv::handleClientResponse(size_t &i)
{
  try
  {
    if (clients[i].totalBytesSent < clients[i].totalToSend)
    {
      int bytesSent
          = send(clients[i].socketFD,
                 clients[i].response.data() + clients[i].totalBytesSent,
                 clients[i].totalToSend - clients[i].totalBytesSent, 0);
      if (bytesSent == -1)
      {
        std::cerr << "Send error: bytesSent == -1 " << std::endl;
        closeConnection(i);
        i--;
      }
      else if (bytesSent == 0)
      {
        std::cout << "bytesSent == 0 " << bytesSent << std::endl;
        clients[i].totalBytesSent = 0;
        clients[i].totalToSend = 0;
        if (clients[i].req.bodyTooLarge == true
            || checkCloseConnectionReq(clients[i].req)
            || checkCloseConnectionResp(clients[i].response))
        {
          closeConnection(i);
          --i;
        }
        clients[i].response.clear();
      }
      else if (bytesSent > 0)
      {
        std::cout << "Bytes sent: " << bytesSent << std::endl;

        clients[i].totalBytesSent += bytesSent;
        if (clients[i].totalBytesSent >= clients[i].totalToSend
            && (clients[i].req.bodyTooLarge == true
                || checkCloseConnectionReq(clients[i].req)
                || checkCloseConnectionResp(clients[i].response)))
        {
          closeConnection(i);
          --i;
        }
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
          || checkCloseConnectionReq(clients[i].req))
      {
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
