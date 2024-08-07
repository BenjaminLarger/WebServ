/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   WebservClientRequest.cpp                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: demre <demre@student.42malaga.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/01 20:07:09 by demre             #+#    #+#             */
/*   Updated: 2024/08/07 16:58:08 by demre            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "GET.hpp"
#include "POST.hpp"
#include "Webserv.hpp"

void Webserv::handleClientRequest(
    size_t i, const std::vector<ServerConfig> &serverConfigs)
{
  (void)serverConfigs;
  char buffer[100000];

  try
  {
    ssize_t bytes_read = read(fds[i].fd, buffer, sizeof(buffer));
    if (bytes_read < 0)
    {
      if (errno != EAGAIN
          && errno
                 != EWOULDBLOCK) // [Isaac] Need to talk about if this exception could break the logic
      {
        closeConnection(i);
        --i;
        throw HttpException(
            "500",
            "Internal Server Error: Data failed to be sent to the client");
      }
    }
    else if (bytes_read == 0)
    {
      closeConnection(i);
      --i;
    }
    else
    {
      // Handle incoming data (e.g., parse HTTP request)
      buffer[bytes_read] = '\0';
      size_t serverIndex = clients[i].serverIndex;

      std::cout << "Request received on serverIndex " << serverIndex
                << ", port " << clients[i].port << ", clients[i].socketFD "
                << clients[i].socketFD
                << ", root: " << serverConfigs[serverIndex].serverRoot
                << std::endl;
      // std::cout << "Request: \n" << buffer << std::endl;

      std::string &clientBuffer = clients[i].req.buffer;
      clientBuffer += buffer;

      // parseClientRequest();
      if (!strncmp("GET ", clientBuffer.c_str(), 4))
        GET method(fds[i].fd, clientBuffer, serverConfigs[serverIndex]);
      else if (!strncmp("POST ", clientBuffer.c_str(), 5))
        POST method(serverIndex, fds[i].fd, clientBuffer);
      else
      {
        clientBuffer.erase();
        throw HttpException("400", "Bad request");
      }
      // closeConnection(i); // commented while testing
      // --i; // commented while testing
    }
    // cleanupClientRequest()
  }
  catch (const HttpException &e)
  {
    std::cerr << RED << "Error: " << e.what() << RESET << '\n';
    sendDefaultErrorPage(fds[i].fd, e.getStatusCode(), e.getErrorMessage(),
                         serverConfigs[clients[i].serverIndex].errorPages);
  }
}
