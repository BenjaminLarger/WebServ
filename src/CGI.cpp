/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGI.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: demre <demre@student.42malaga.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/10 14:38:48 by demre             #+#    #+#             */
/*   Updated: 2024/08/15 20:55:15 by demre            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Webserv.hpp"
#include "core.hpp"

extern char **environ;

static void checkFileAndScriptExecPaths(std::string const &filePath,
                                        std::string const &script)
{
  // Check for PHP and python executables
  if (script == "php")
  {
    if (access("/usr/bin/php", F_OK) == -1)
      throw HttpException(500, "PHP executable not found at /usr/bin/php");
  }
  else if (script == "py")
  {
    if (access("/usr/bin/python3", F_OK) == -1)
      throw HttpException(500,
                          "Python executable not found at /usr/bin/python3");
  }
  else
    throw HttpException(500, "Unsupported script type to execute: " + script);

  // Check if script file to execute exists
  if (access((char *)filePath.c_str(), F_OK) == -1)
    throw HttpException(404,
                        "Script file to execute not found at: " + filePath);
}

void Webserv::executeScript(std::string const &filePath,
                            std::string const &script, int &clientFD)
{
  std::cout << "Executing: " << filePath << std::endl;

  checkFileAndScriptExecPaths(filePath, script);

  int pipefd[2];
  if (pipe(pipefd) == -1)
    throw HttpException(500, "Pipe error when executing " + filePath);

  pid_t pid = fork();
  if (pid == -1)
    throw HttpException(500, "Fork error when executing " + filePath);

  if (pid == 0) // Execute script in child process
  {
    if (close(pipefd[0]) == -1 || dup2(pipefd[1], STDOUT_FILENO) == -1
        || close(pipefd[1]) == -1)
      exit(1);

    if (script == "php")
    {
      char *argv[] = {(char *)"php", (char *)filePath.c_str(), NULL};
      execve("/usr/bin/php", argv, environ);
    }
    else if (script == "py")
    {
      char *argv[] = {(char *)"python3", (char *)filePath.c_str(), NULL};
      execve("/usr/bin/python3", argv, environ);
    }
    exit(1);
  }
  else // Add read end of the pipe to the pollfd vector in parent process
  {
    close(pipefd[1]);

    setNonBlocking(pipefd[0]);

    // Store the pid to manage multiple processes
    pidMap[pipefd[0]] = pid;

    pollfd pfd;
    pfd.fd = pipefd[0];
    pfd.events = (POLLIN | POLLHUP);
    pfd.revents = 0;

    // Associate the pipe FD with the client connection
    clientScriptMap[pipefd[0]] = clientFD;
    // std::cout << GREEN << "Adding to clientScriptMap " << RESET << pipefd[0]
    //           << " " << clientFD << std::endl;
    fds.push_back(pfd);

    // Add a dummy client info for the listening socket
    ClientInfo ci;
    ci.socketFD = pipefd[0];
    ci.port = -1;
    clients.push_back(ci);
  }
}

std::string Webserv::generateCgiOutputHtmlPage(const std::string &output)
{
  std::ostringstream htmlStream;

  htmlStream << "<html>\n"
             << "<head><title>CGI Script Output</title></head>\n"
             << "<body>\n"
             << "<h1>CGI script correctly executed</h1>\n"
             << "<p>Content length is " << output.size() << "</p>\n"
             << "<p>CGI output: " << output << "</p>\n"
             << "</body>\n"
             << "</html>";

  return (htmlStream.str());
}

size_t Webserv::findClientIndexFromPipeFD(int pipeFD)
{
  int clientFD = clientScriptMap[pipeFD];

  size_t j = 0;
  while (j < clients.size())
  {
    if (clients[j].socketFD == clientFD)
      break;
    ++j;
  }

  return (j);
}

size_t Webserv::findClientIndexFromClientFD(int clientFD)
{
  size_t j = 0;
  while (j < clients.size())
  {
    if (clients[j].socketFD == clientFD)
      break;
    ++j;
  }

  return (j);
}

void Webserv::handleScriptOutput(size_t &i)
{
  try
  {
    size_t j = findClientIndexFromPipeFD(fds[i].fd);
    int clientFD = clients[j].socketFD;

    char buffer[10];
    ssize_t bytesRead = read(fds[i].fd, buffer, sizeof(buffer) - 1);
    std::cout << "A bytesRead: " << bytesRead << std::endl;
    if (bytesRead > 0)
    {
      buffer[bytesRead] = '\0';

      clients[j].responseBuffer += buffer;

      std::cout << "(bytesRead: " << bytesRead << ". Read from pipe: '"
                << buffer << "'" << std::endl;
    }
    else if (bytesRead < 0)
    {
      closePipe(i);
      --i;
      j = findClientIndexFromClientFD(clientFD);
      closeConnection(j);
      throw HttpException(500, "Error reading from pipe");
    }
    else if (bytesRead == 0)
    {
      std::cout << "(bytesRead == 0)" << std::endl;

      clients[j].response = composeOkHtmlResponse(
          generateCgiOutputHtmlPage(clients[j].responseBuffer),
          clients[j].req.buffer);
      clients[j].responseBuffer.clear();
      closePipe(i);
      --i;
    }
  }
  catch (const HttpException &e)
  {
    std::cerr << RED << "Error: " << e.getStatusCode() << " " << e.what()
              << RESET << '\n';

    clients[i].response = composeErrorHtmlPage(
        e.getStatusCode(), getReasonPhrase(e.getStatusCode()),
        clients[i].client_serverConfig.errorPages);
  }
}