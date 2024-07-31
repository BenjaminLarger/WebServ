/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   WebservUtils.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blarger <blarger@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/08 16:12:10 by blarger           #+#    #+#             */
/*   Updated: 2024/07/31 14:56:18 by blarger          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Webserv.hpp"

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
    return -1;
  return fcntl(fd, F_SETFL, flags | O_NONBLOCK);
}

/**
  This function aims to check if he config file has an equal number of curly
  brace per server, counts the number of server and save inside the class as
  numberOfServers.
*/
void Webserv::countAndParseServer(const char *filename)
{
  std::ifstream file(filename);
  int openCurlyBraceCount = 0;
  int closeCurlyBraceCount = 0;
  size_t pos;

  this->numberOfServers = 0;

  if (!file.is_open())
    throw(std::runtime_error("Could not open the configuration file!"));

  for (std::string line; std::getline(file, line);)
  {
    std::istringstream isLine(line);
    std::string key;
    isLine >> key;
    if (key == "server")
    {
      openCurlyBraceCount = 0;
      closeCurlyBraceCount = 0;
      this->numberOfServers++;
      pos = line.find('{');
      if (pos != std::string::npos)
        openCurlyBraceCount++;
      pos = line.find('}');
      if (pos != std::string::npos)
        closeCurlyBraceCount++;
      for (std::string line; std::getline(file, line)
                             && openCurlyBraceCount != closeCurlyBraceCount;)
      {
        std::istringstream isLine(line);
        pos = line.find('{');
        if (pos != std::string::npos)
          openCurlyBraceCount++;
        pos = line.find('}');
        if (pos != std::string::npos)
          closeCurlyBraceCount++;
      }
    }
  }
  if (closeCurlyBraceCount != openCurlyBraceCount)
    throw(std::runtime_error("parsing config: unclosed brace!"));
}

/*
  This code prepares a list of file descriptors to be monitored for readability
  (i.e., data is available to be read) using the poll system call. The poll call
  itself would be made later in the code, where it would check the fds vector to
  see if any of the file descriptors are ready for the specified operation
  (POLLIN in this case).
*/
std::vector<pollfd> Webserv::initializePollFDSWithServerSocket(int serverFD)
{
  std::vector<pollfd> fds;
  struct pollfd temp_fd;
  temp_fd.fd = serverFD;
  temp_fd.events = POLLIN;
  temp_fd.revents = 0;
  (void)temp_fd;
  fds.push_back(temp_fd);
  return (fds);
}

void Webserv::monitorSocketEvents(std::vector<pollfd> &fds, int serverFD)
{
  /* The poll() function shall identify those file descriptors on
	which an application can read or write data, or on which certain
	events have occurred */
  //std::cout << BLUE << "Monitoring...\n" << RESET;
  //std::cout << "POLLIN = " << POLLIN << std::endl;
  int pollCount = poll(fds.data(), fds.size(), -1);
  if (pollCount < 0)
  {
    close(serverFD);
    throw(std::runtime_error("poll failed"));
  }
  else if (pollCount == 0)
  {
    //std::cout << RED << "Timeout occured !\n" << RESET;
  }
}

struct pollfd Webserv::setNewTempFDStruct(int newSocket)
{
  struct pollfd newTempFD;

  newTempFD.fd = newSocket;
  newTempFD.events = POLLIN;
  newTempFD.revents = 0;
  return (newTempFD);
}

/*
  Manages new connections and incoming data on existing connections in a
  non-blocking manner, allowing the server to handle multiple clients
  simultaneously.
 */
void Webserv::processConnectionData(int serverFD, std::vector<pollfd> &fds,
                                    size_t &i,
                                    std::map<size_t, std::string> &buffers)
{
  char buffer[1024];
  // memset(buffer, 0, sizeof(buffer));
  int bytes_read = read(fds[i].fd, buffer, sizeof(buffer));
  buffer[bytes_read] = '\0';
  if (bytes_read <= 0)
  {
    if (bytes_read == 0)
      std::cout << "Connection closed: " << fds[i].fd << std::endl;
    else
      throw(std::runtime_error("read failed"));
    ;
    close(fds[i].fd);
    fds.erase(fds.begin() + i);
    --i;
  }
  else
  {
    //std::cout << "Received data: " << buffer << std::endl;
    // Echo the data back to the client
    processClientInput(buffer, serverFD, fds[i].fd, buffers[i]);
  }
}

void Webserv::serverListeningLoop(int serverFD)
{
  std::vector<pollfd> fds = initializePollFDSWithServerSocket(serverFD);
  std::map<size_t, std::string> buffers;

  while (true)
  {
    //Check if there is a new client connection
    monitorSocketEvents(fds, serverFD);
    for (size_t i = 0; i < fds.size(); ++i)
    {
      // evaluates to true if the i-th file descriptor has incoming data
      // available for reading.
      if (fds[i].revents & POLLIN)
      {
        std::cout << CYAN << "New event detected !\n" << RESET;
        //There is a new event detected
        if (fds[i].fd == serverFD)
        {
          std::cout << GREEN << "New connexion detected !\n" << RESET;
          // Accept new connections
          while (true)
          {
            //std::cout << "Enter in while(true)\n";
            int newSocket = accept(serverFD, NULL, NULL);
            if (newSocket < 0)
            {
              if (errno == EWOULDBLOCK
                  || errno
                         == EAGAIN) //We cannot check errno value => find another way to break the loop
              {
                std::cout << "No more incoming connections !\n";
                // No more incoming connections
                break;
              }
              else
              {
                close(serverFD);
                throw(std::runtime_error("Failed to accept connection."));
              }
            }
            std::cout << "New connection accepted: " << newSocket << std::endl;
            //Set the newly created socket (newSocket) to non-blocking mode
            this->setNonBlocking(newSocket);
            //enable the server to monitor the new socket
            struct pollfd newTempFD = setNewTempFDStruct(newSocket);
            fds.push_back(
                newTempFD); // We might have to drop this socket when the connection is closed ?
          }
        }
        else
        {
          // Handle data from an existing connection
          processConnectionData(serverFD, fds, i, buffers);
          //break;
        }
      }
    }
  }
}
