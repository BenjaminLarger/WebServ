/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGI.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: demre <demre@student.42malaga.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/10 14:38:48 by demre             #+#    #+#             */
/*   Updated: 2024/08/11 17:41:05 by demre            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "CGI.hpp"
#include "core.hpp"

extern char **environ;

std::string executeScript(std::string const &filePath,
                          std::string const &script)
{
  std::cout << "Executing: " << filePath << std::endl;

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

  int pipefd[2];
  if (pipe(pipefd) == -1)
    throw HttpException(500, "Pipe error when executing " + filePath);

  pid_t pid = fork();
  if (pid == -1)
    throw HttpException(500, "Fork error when executing " + filePath);

  if (pid == 0) // Execute script in child process
  {
    close(pipefd[0]);

    if (dup2(pipefd[1], STDOUT_FILENO) == -1)
    {
      perror("dup2");
      return ("");
    }
    close(pipefd[1]);

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

    perror("execve"); //cannot use perror
    return ("");      // catch error 500 if execution failed
  }
  else // Read the output from the read end of the pipe in parent process
  {
    int status;

    close(pipefd[1]);

    char buffer[4096];
    ssize_t bytesRead;
    std::ostringstream output;
    while ((bytesRead = read(pipefd[0], buffer, sizeof(buffer) - 1)) > 0)
    {
      buffer[bytesRead] = '\0';
      output << buffer;
    }

    close(pipefd[0]);
    waitpid(pid, &status, 0);

    return (output.str());
  }
}
