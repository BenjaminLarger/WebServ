/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   WebservSignal.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: demre <demre@student.42malaga.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/01 11:37:18 by blarger           #+#    #+#             */
/*   Updated: 2024/08/13 13:58:56 by demre            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Webserv.hpp"

Webserv *Webserv::instance = NULL;

void Webserv::sigInt(int code) { instance->handleSigInt(code); }

void Webserv::handleSigInt(int code)
{
  std::cout << RED << "\n>>> SIGINT received [" << code << "]\n" << RESET;
  if (fds[0].fd != 0)
  {
    size_t size = fds.size();
    for (size_t i = 0; i < size; ++i) // server + incoming clients
    {
      std::cout << "Closing fd: " << fds[i].fd << std::endl;
      close(fds[i].fd);
    }
  }
  exit(EXIT_SUCCESS);
}
