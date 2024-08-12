/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   WebservClientResponse.cpp                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: demre <demre@student.42malaga.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/01 20:07:09 by demre             #+#    #+#             */
/*   Updated: 2024/08/12 20:03:32 by demre            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "DELETE.hpp"
#include "GET.hpp"
#include "POST.hpp"
#include "Webserv.hpp"
#include "core.hpp"

/* void Webserv::handleClientResponse(size_t i)
{
  try
  {
    // check to which client corresponds fds[i].fd

    int status;
    std::ostringstream output;

    char buffer[4096];
    ssize_t bytesRead;
    while ((bytesRead = read(fds[i].fd, buffer, sizeof(buffer) - 1)) > 0)
    {
      buffer[bytesRead] = '\0';
      output << buffer;
    }

    close(fds[i].fd);
    // fds.erase(fds.begin() + i);
    // clients.erase(clients.begin() + i);

    waitpid(pid, &status, 0);

    if (WIFEXITED(status) && WEXITSTATUS(status) == 0)
    {
      output.str();
    }
    else
      throw HttpException(500, "Script execution failed");
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
 */