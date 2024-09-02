/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   WebservSignal.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blarger <blarger@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/01 11:37:18 by blarger           #+#    #+#             */
/*   Updated: 2024/09/02 17:40:01 by blarger          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Webserv.hpp"
#include "utils.hpp"

Webserv *Webserv::instance = NULL;

void Webserv::sigInt(int code) { instance->handleSigInt(code); }

void Webserv::handleSigInt(int code)
{
  std::cout << RED << "\n>>> SIGINT received [" << code << "]\n" << RESET;
  if (fds.size() > 0 && fds[0].fd != 0)
  {
    size_t size = fds.size();
    for (size_t i = 0; i < size; ++i) // server + incoming clients
    {
      std::cout << "Closing fd: " << fds[i].fd << std::endl;
      close(fds[i].fd);
    }
  }
	deleteLogContentFile(std::string(LOG_DIR_PATH) + "form.log");
	deleteLogContentFile(std::string(LOG_DIR_PATH) + "cookies.log");
  exit(EXIT_SUCCESS);
}
