/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGI.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blarger <blarger@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/10 14:38:48 by demre             #+#    #+#             */
/*   Updated: 2024/08/20 09:35:43 by blarger          ###   ########.fr       */
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

static void setEnvironmentVariables(std::string const &filePath,
                                    std::string const &queryString)
{
  if (!queryString.empty())
  {
    if (setenv("QUERY_STRING", queryString.c_str(), 1) == -1)
      throw HttpException(500, "Failed to setenv QUERY_STRING");
  }

  if (setenv("PATH_INFO", filePath.c_str(), 1) == -1)
    throw HttpException(500, "Failed to setenv PATH_INFO");
}

void Webserv::executeScript(std::string const &filePath,
                            std::string const &script,
                            std::string const &queryString, int &clientFD)
{
  std::cout << "Executing: " << filePath;
  if (queryString.size())
    std::cout << " + " << queryString;
  std::cout << std::endl;

  setenv("PATH_INFO", filePath.c_str(), 1);

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

    setEnvironmentVariables(filePath, queryString);

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

/* void Webserv::executeScript(std::string const &filePath,
                            std::string const &script, int &clientFD, std::string &response)
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
	else
	std::cout << RED << "errno : " << strerror(errno) << RESET << std::endl;
		
		
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
} */