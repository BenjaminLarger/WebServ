/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   WebservClientResponse.cpp                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: demre <demre@student.42malaga.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/01 20:07:09 by demre             #+#    #+#             */
/*   Updated: 2024/08/13 19:00:20 by demre            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "DELETE.hpp"
#include "GET.hpp"
#include "POST.hpp"
#include "Webserv.hpp"
#include "core.hpp"

void Webserv::handleClientResponse(size_t i)
{
  try
  {
    sendRGeneric(clients[i].socketFD, clients[i].response);
    clients[i].response.clear();
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
