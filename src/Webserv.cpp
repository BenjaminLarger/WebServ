/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Webserv.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: demre <demre@student.42malaga.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/01 20:06:23 by demre             #+#    #+#             */
/*   Updated: 2024/08/12 20:47:52 by demre            ###   ########.fr       */
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

    // add try / catch around each incoming connection?
    for (size_t i = 0; i < fds.size(); ++i)
    {
      //withdrawWriteCapability(i, clients[i].buffer);

      if (fds[i].revents & POLLIN)
      {
        std::cout << CYAN << "New POLLIN event detected\n" << RESET;
        if (i < serverConfigs.size())
        {
          // New connection request on listening socket
          std::cout << GREEN << "New connection detected\n" << RESET;
          handleNewConnection(i, serverConfigs);
        }
        else
        {
          try
          {
            // Check if client socket (is not a pipefd)
            if (clientScriptMap.find(fds[i].fd) == clientScriptMap.end())
            {
              std::cout << GREEN << "New client request detected\n" << RESET;
              handleClientRequest(i, serverConfigs);
            }
            // Check if script output pipe
            else if (clientScriptMap.find(fds[i].fd) != clientScriptMap.end())
            {
              std::cout << GREEN << "New script output pipe ready\n" << RESET;
              char buffer[1024];
              ssize_t bytesRead = read(fds[i].fd, buffer, sizeof(buffer) - 1);
              if (bytesRead > 0)
              {
                buffer[bytesRead] = '\0';

                int clientFD = clientScriptMap[fds[i].fd];
                std::string response = composeOkHtmlResponse(buffer);
                sendRGeneric(clientFD, response);

                close(fds[i].fd);
                fds.erase(fds.begin() + i); // Remove the FD from the vector
                clientScriptMap.erase(fds[i].fd); // Remove the mapping
                --i;                              // Adjust index after removal
              }
              else
              {
                // Handle script completion or pipe closure
                close(fds[i].fd);
                fds.erase(fds.begin() + i); // Remove the FD from the vector
                clientScriptMap.erase(fds[i].fd); // Remove the mapping
                --i;                              // Adjust index after removal
              }
            }
          }
          catch (const HttpException &e)
          {
            std::cerr << RED << "Error: " << e.getStatusCode() << " "
                      << e.what() << RESET << '\n';
          }
        }
      }
      else if (fds[i].revents & POLLOUT) // when ?
      {
        std::cout << CYAN << "New POLLOUT event detected\n" << RESET;
        try
        {
          // handleClientResponse(i);
        }
        catch (const HttpException &e)
        {
          std::cerr << RED << "Error: " << e.getStatusCode() << " " << e.what()
                    << RESET << '\n';
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

// Close connection and remove from pollfd array
void Webserv::closeConnection(size_t index)
{
  std::cout << "Connection closed: " << fds[index].fd << std::endl;
  close(fds[index].fd);
  fds.erase(fds.begin() + index);
  clients.erase(clients.begin() + index);
}
