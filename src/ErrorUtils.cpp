/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ErrorUtils.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: demre <demre@student.42malaga.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/01 19:54:52 by demre             #+#    #+#             */
/*   Updated: 2024/08/01 19:54:58 by demre            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Webserv.hpp"
#include "utils.hpp"

void sendErrorResponse(int clientSocket, std::string statusCode,
                       const std::string &statusMessage)
{
  std::string response
      = "HTTP/1.1 " + statusCode + " " + statusMessage + "\r\n";
  response += "Content-Type: text/plain\r\n\r\n";
  response += statusMessage + "\n";

  if (sendall(clientSocket, response.c_str(), response.size()) == -1)
    perror("Data failed to be sent to the client");
}
