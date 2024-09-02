/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Webserv.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blarger <blarger@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/01 20:06:23 by demre             #+#    #+#             */
/*   Updated: 2024/09/02 17:40:47 by blarger          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Webserv.hpp"
#include "core.hpp"

Webserv::Webserv(std::vector<ServerConfig> &serverConfigs)
{
  createServers(serverConfigs);

  instance = this;
  signal(SIGINT, Webserv::sigInt);

  // Server's main listening loop to handle incoming connections
  while (true)
  {
    int pollCount = poll(fds.data(), fds.size(), -1);
    if (pollCount < 0)
    {
      std::cerr << "Failed to poll." << std::endl;
      continue;
    }
    if (pollCount == 0)
      continue;

    for (size_t i = 0; i < fds.size(); ++i)
    {
      if (fds[i].revents & (POLLIN | POLLHUP))
      {
        if (i < serverConfigs.size())
        {
          // New connection request on listening socket
          std::cout << GREEN << "New connection detected " << RESET << fds[i].fd
                    << std::endl;
          handleNewConnection(i);
        }
        else
        {
          try
          {
            if (!(fds[i].revents & POLLHUP)
                && clientScriptMap.find(fds[i].fd) == clientScriptMap.end())
            {
              std::cout << GREEN << "New client chunk request detected "
                        << RESET << fds[i].fd << std::endl;
              handleClientRequest(i, serverConfigs);
            }
            // Check if script output pipe
            else if (clientScriptMap.find(fds[i].fd) != clientScriptMap.end())
            {
              readAndHandleScriptOutput(i);
            }
          }
          catch (const HttpException &e)
          {
            std::cerr << RED << "Error: " << e.getStatusCode() << " "
                      << e.what() << RESET << std::endl;
          }
        }
      }
      else if (fds[i].revents & POLLOUT)
      {
        try
        {
          if (clients[i].response.size())
            handleClientResponse(i);
        }
        catch (const HttpException &e)
        {
          std::cerr << RED << "Error: " << e.getStatusCode() << " " << e.what()
                    << RESET << std::endl;
        }
      }
    }
  }
}

/* --------------DESTRUCTORS */

Webserv::~Webserv(void)
{
  for (size_t i = 0; i < fds.size(); ++i) // server + incoming
  {
    std::cout << "Closing: " << fds[i].fd << std::endl;
    close(fds[i].fd);
  }
	deleteLogContentFile(std::string(LOG_DIR_PATH) + "form.log");
	deleteLogContentFile(std::string(LOG_DIR_PATH) + "cookies.log");
}

// --------------MEMBER

/**
@param: int fd - File descriptor for which to set non-blocking mode.
@description: Sets the file descriptor specified by 'fd' to non-blocking mode.
This is achieved by retrieving the current flags of 'fd' using F_GETFL,
then modifying these flags to include O_NONBLOCK, and finally setting
the modified flags back on 'fd' using F_SETFL. If any step fails, the function
returns -1 to indicate an error.
@return: int - Returns 0 on success, or -1 on failure.
*/
int Webserv::setNonBlocking(int fd)
{
  int flags = fcntl(fd, F_GETFL, 0);
  if (flags < 0)
    return (-1);
  return (fcntl(fd, F_SETFL, flags | O_NONBLOCK));
}

size_t Webserv::findClientIndexOfConnectionFromPipeFD(int pipeFD)
{
  int clientFD = clientScriptMap[pipeFD];

  size_t j = 0;
  while (j < clients.size())
  {
    if (clients[j].socketFD == clientFD)
      break;
    j++;
  }

  return (j);
}

size_t Webserv::findClientIndexFromFD(int fd)
{
  size_t j = 0;
  while (j < clients.size())
  {
    if (clients[j].socketFD == fd)
      break;
    j++;
  }

  return (j);
}
