/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   WebservClientResponse.cpp                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blarger <blarger@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/01 20:07:09 by demre             #+#    #+#             */
/*   Updated: 2024/08/13 16:01:58 by blarger          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "DELETE.hpp"
#include "GET.hpp"
#include "POST.hpp"
#include "Webserv.hpp"
#include "core.hpp"

void Webserv::handleClientResponse(size_t i)
{
  (void)i;
  try
  {
		std::cout << GREEN << "Sending request to fd " << clients[i].socketFD << RESET << std::endl;
		sendRGeneric(clients[i].socketFD, clients[i].response);
  }
  catch (const HttpException &e)
  {
    std::cerr << RED << "Error: " << e.getStatusCode() << " " << e.what()
              << RESET << '\n';

    // sendDefaultErrorPage(fds[i].fd, e.getStatusCode(),
    //                      getReasonPhrase(e.getStatusCode()),
    //                      serverConfigs[clients[i].serverIndex].errorPages);
  }
}
