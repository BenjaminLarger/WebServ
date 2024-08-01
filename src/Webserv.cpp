/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Webserv.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: demre <demre@student.42malaga.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/31 18:01:57 by demre             #+#    #+#             */
/*   Updated: 2024/07/31 18:02:05 by demre            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Webserv.hpp"
#include "GET.hpp"
#include "POST.hpp"

/* --------------CONSTRUCTORS */
//The socket() function shall create an unbound socket in a communications domain,
//and return a file descriptor that can be used in later function calls that operate on sockets.

Webserv::Webserv(std::vector<ServerConfig> serverConfigs)
// : port(config.getPort()), serverFD(socket(AF_INET, SOCK_STREAM, 0)),
//   optval(1)
{
  // Create and set up listening sockets for all servers
  for (std::vector<ServerConfig>::size_type i = 0; i < serverConfigs.size();
       ++i)
  {
    // Create socket and check if successful
    int serverFD = socket(AF_INET, SOCK_STREAM, 0);
    if (serverFD < 0)
      throw(std::range_error("Failed to create socket."));

    // serverFds.push_back(serverFD); // don't know if needed

    // Set the socket to non-blocking mode
    if (setNonBlocking(serverFD) < 0)
    {
      close(serverFD);
      throw(std::range_error("Failed to set non-blocking."));
    }

    struct sockaddr_in serverAddress = {};
    // memset(&serverAddress, 0, sizeof(serverAddress)); // not sure if needed
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = INADDR_ANY;
    serverAddress.sin_port = htons(serverConfigs[i].getPort());

    // Set socket options to allow reuse of local addresses
    int optval = 1;
    if (setsockopt(serverFD, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval))
        < 0)
    {
      close(serverFD);
      throw(std::range_error("setsockopt(SO_REUSEADDR) failed"));
    }

    // Link the socket to the specified address and port
    if (bind(serverFD, (struct sockaddr *)&serverAddress, sizeof(serverAddress))
        < 0)
    {
      close(serverFD);
      throw(std::range_error("Failed to bind to port."));
    }

    // Start listening for incoming connections on the socket
    if (listen(serverFD, 10) < 0)
    {
      close(serverFD);
      throw(std::range_error("Failed to listen on socket."));
    }

    // Add the listening socket to the pollfd vector
    pollfd pfd;
    pfd.fd = serverFD;
    pfd.events = POLLIN;
    pfd.revents = 0;
    fds.push_back(pfd);

    // Add a dummy client info for the listening socket
    ClientInfo ci;
    ci.socketFD = serverFD;
    ci.serverIndex = i; // Associate with the respective server
    clients.push_back(ci);
  }
  // Enter the server's main listening loop to handle incoming connections
  // serverListeningLoop(serverFD);

  // Main server loop
  while (true)
  {
    // int pollCount = poll(&fds[0], fds.size(), -1);
    int pollCount = poll(fds.data(), fds.size(), -1);
    if (pollCount < 0)
      throw(std::runtime_error("Failed to poll."));

    // add try / catch around each incoming connection?
    for (size_t i = 0; i < fds.size(); ++i)
    {
      // evaluates to true if the i-th file descriptor has incoming data available for reading.
      if (fds[i].revents & POLLIN)
      {
        std::cout << CYAN << "New event detected\n" << RESET;
        if (i < serverConfigs.size())
        // if (clients[i].socketFD == fds[i].fd)
        {
          // New connection request on listening socket
          std::cout << GREEN << "New connection detected\n" << RESET;
          handleNewConnection(i, serverConfigs);
        }
        else
        {
          // Data from a connected client ~= processConnectionData()
          handleClientData(i, serverConfigs);
        }
      }
    }
  }

  // Close all listening sockets // servers
  std::cout << "Close all listening sockets" << std::endl;
  for (size_t i = 0; i < serverConfigs.size(); ++i) // fds.size()?
  {
    std::cout << "Closing: " << fds[i].fd << std::endl;
    close(fds[i].fd);
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

void Webserv::handleNewConnection(
    size_t i, const std::vector<ServerConfig> &serverConfigs)
{
  while (true)
  {
    int newSocket = accept(fds[i].fd, NULL, NULL);
    if (newSocket < 0)
    {
      if (errno == EAGAIN || errno == EWOULDBLOCK)
      {
        // No more incoming connections at the moment
        break;
      }
      else
      {
        std::cerr << "accept failed: " << strerror(errno)
                  << " (errno: " << errno << ")" << std::endl;
        break;
      }
    }

    if (setNonBlocking(newSocket) < 0)
    {
      close(newSocket);
      perror("Failed to set non-blocking for new connection");
      continue;
    }

    std::cout << "New connection accepted: " << newSocket << ", on port: "
              << serverConfigs[clients[i].serverIndex].getPort() << std::endl;

    // Add the new socket to the pollfd vector
    pollfd pfd;
    pfd.fd = newSocket;
    pfd.events = POLLIN;
    pfd.revents = 0;
    fds.push_back(pfd);

    // Add the new client info to the clients vector
    ClientInfo ci;
    ci.socketFD = newSocket;
    ci.serverIndex = clients[i].serverIndex;
    clients.push_back(ci);
  }
}

void Webserv::handleClientData(size_t i,
                               const std::vector<ServerConfig> &serverConfigs)
{
  char buffer[1024];
  ssize_t bytes_read = read(fds[i].fd, buffer, sizeof(buffer));
  if (bytes_read < 0)
  {
    if (errno != EAGAIN && errno != EWOULDBLOCK)
    {
      perror("Failed to read from client.");
      // Close connection and remove from pollfd array
      close(fds[i].fd);
      fds.erase(fds.begin() + i);
      clients.erase(clients.begin() + i);
      --i;
    }
  }
  else if (bytes_read == 0)
  {
    std::cout << "Connection closed: " << fds[i].fd << std::endl;
    // Close connection and remove from pollfd array
    close(fds[i].fd);
    fds.erase(fds.begin() + i);
    clients.erase(clients.begin() + i);
    --i;
  }
  else
  {
    // Handle incoming data (e.g., parse HTTP request)
    buffer[bytes_read] = '\0';
    size_t serverIndex = clients[i].serverIndex;

    std::cout << "Received on serverIndex " << serverIndex << ", port "
              << serverConfigs[serverIndex].getPort()
              << ", clients[i].socketFD " << clients[i].socketFD << ": "
              << buffer << std::endl;

    std::string &clientBuffer = clients[i].buffer;
    clientBuffer += buffer; // buffers[i] == staticBuffer

    if (!strncmp("GET ", clientBuffer.c_str(), 4))
    {
      GET method(serverIndex, fds[i].fd, clients[i].buffer);
    }
    // else if (!strncmp("PUT ", buffers[i].c_str(), 4))
    //   processPutMethod(serverFD, fds[i].fd);
    // else if (!strncmp("DELETE ", buffers[i].c_str(), 7))
    //   processDeleteMethod(serverFD, fds[i].fd);
    // else if (!strncmp("POST ", buffers[i].c_str(), 5))
    //   POST method(serverIndex, fds[i].fd, buffers[i]);
    else
    {
      clientBuffer.erase();
      std::cout << RED << "Unknown instruction received!"
                << " staticBuffer = " << clientBuffer << RESET << std::endl;
    }
  }
}

void Webserv::closeConnection(size_t index)
{
  close(fds[index].fd);
  fds.erase(fds.begin() + index);
  clients.erase(clients.begin() + index);
}
