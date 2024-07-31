/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ClientProcessMethods.cpp                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blarger <blarger@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/03 08:37:10 by blarger           #+#    #+#             */
/*   Updated: 2024/07/31 15:47:54 by blarger          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/POST.hpp"
#include "GET.hpp"

void processPutMethod(int serverFD, int clientFD)
{

  (void)serverFD;
  (void)clientFD;
  std::cout << GREEN << "PUT HTTML method instructions received!" << RESET
            << std::endl;
  write(clientFD, "PUT client info\n", 14);
}

void processDeleteMethod(int serverFD, int clientFD)
{
  (void)serverFD;
  (void)clientFD;
  std::cout << GREEN << "DELETE HTTML method instructions received!" << RESET
            << std::endl;
  write(clientFD, "DELETE client info\n", 17);
}

void Webserv::processClientInput(std::string clientInput, int serverFD,
                                 int clientFD, std::string &staticBuffer)
{
  staticBuffer += clientInput;

  if (!strncmp("GET ", staticBuffer.c_str(), 4))
  {
    GET method(*this, serverFD, clientFD, staticBuffer);
  }
  else if (!strncmp("PUT ", staticBuffer.c_str(), 4))
    processPutMethod(serverFD, clientFD);
  else if (!strncmp("DELETE ", staticBuffer.c_str(), 7))
    processDeleteMethod(serverFD, clientFD);
  else if (!strncmp("POST ", staticBuffer.c_str(), 5))
    POST method(*this, serverFD, clientFD, staticBuffer);
  else
  {
    staticBuffer.erase();
    std::cout << RED << "Unknown instruction received!"
              << " staticBuffer = " << staticBuffer << RESET << std::endl;
  }
}
