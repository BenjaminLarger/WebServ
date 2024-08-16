/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Webserv.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: demre <demre@student.42malaga.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/01 20:06:23 by demre             #+#    #+#             */
/*   Updated: 2024/08/16 17:57:07 by demre            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Webserv.hpp"
#include "core.hpp"
/* --------------CONSTRUCTORS */
//The socket() function shall create an unbound socket in a communications domain,
//and return a file descriptor that can be used in later function calls that operate on sockets.

Webserv::Webserv(std::vector<ServerConfig> &serverConfigs)
{
  createServers(serverConfigs);

  instance = this;
  signal(SIGINT, Webserv::sigInt);
  std::cout << "fds size = " << fds.size() << std::endl;

  // Server's main listening loop to handle incoming connections
  while (true)
  {
    int pollCount = poll(fds.data(), fds.size(), -1);
    if (pollCount < 0)
      throw(std::runtime_error("Failed to poll."));
    if (pollCount == 0)
      continue;

    // Check for terminated child processes
    checkTerminatedProcesses();

    // add try / catch around each incoming connection?
    for (size_t i = 0; i < fds.size(); ++i)
    {
      //withdrawWriteCapability(i, clients[i].buffer);

      if (fds[i].revents & (POLLIN | POLLHUP))
      {
        std::cout << CYAN << "New POLLIN event detected " << RESET << fds[i].fd
                  << std::endl;
        if (i < serverConfigs.size())
        {
          // New connection request on listening socket
          std::cout << GREEN << "New connection detected " << RESET << fds[i].fd
                    << std::endl;
          handleNewConnection(i, serverConfigs);
        }
        else
        {
          try
          {
            // Check if client socket (is not a pipefd)
            if (!(fds[i].revents & POLLHUP)
                && clientScriptMap.find(fds[i].fd) == clientScriptMap.end())
            {
              std::cout << GREEN << "New client request detected " << RESET
                        << fds[i].fd << std::endl;
              handleClientRequest(i, serverConfigs);
            }
            // Check if script output pipe
            else if (clientScriptMap.find(fds[i].fd) != clientScriptMap.end())
            {
              std::cout << GREEN << "New script output pipe ready " << RESET
                        << fds[i].fd << std::endl;
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
        // std::cout << CYAN << "New " << RED << POLLOUT << CYAN
        //           << " event detected" << RESET << std::endl;
        try
        {
          if (clients[i].response.size())
            handleClientResponse(i);
          // clean clients[i].response after send
        }
        catch (const HttpException &e)
        {
          std::cerr << RED << "Error: " << e.getStatusCode() << " " << e.what()
                    << RESET << std::endl;
        }
      }
      if (fds[i].revents & POLLOUT) // when ?
      {
        // std::cout << CYAN << "New " << RED << POLLOUT << CYAN
        // 					<< " event detected" << RESET << std::endl;
        try
        {
          if (clients[i].response.size())
          {
            handleClientResponse(i);
          }
          // else
          // 	std::cout << RED << "Client " << clients[i].socketFD << " has no response" << RESET << std::endl;
          // clean clients[i].response after send
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
  std::cout << "Webserv Destructor called" << std::endl;
  for (size_t i = 0; i < fds.size(); ++i) // server + incoming
  {
    std::cout << "Closing: " << fds[i].fd << std::endl;
    close(fds[i].fd);
  }
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
