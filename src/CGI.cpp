/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGI.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: demre <demre@student.42malaga.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/10 14:38:48 by demre             #+#    #+#             */
/*   Updated: 2024/07/10 15:47:30 by demre            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "CGI.hpp"

std::string executePhp(std::string phpFile)
{
  // Check if PHP executable exists
  if (access("/usr/bin/php", F_OK) == -1)
  {
    std::cerr << "Error: PHP executable not found at /usr/bin/php" << std::endl;
    return ("");
  }

  // Check if PHP file exists
  std::string phpFilePath = "cgi-bin/php";
  if (phpFile[0] != '/')
    phpFilePath += '/';
  phpFilePath += phpFile;
  if (access((char *)phpFilePath.c_str(), F_OK) == -1)
  {
    std::cerr << "Error: target php file not found at: " << phpFilePath
              << std::endl;
    return ("");
  }

  int pipefd[2];
  if (pipe(pipefd) == -1)
  {
    perror("pipe");
    return ("");
  }

  pid_t pid = fork();
  if (pid == -1)
  {
    perror("fork");
    return ("");
  }

  if (pid == 0) // Execute php script in child process
  {
    close(pipefd[0]);

    if (dup2(pipefd[1], STDOUT_FILENO) == -1)
    {
      perror("dup2");
      return ("");
    }
    close(pipefd[1]);
    char *argv[] = {(char *)"php", (char *)phpFilePath.c_str(), NULL};
    execve("/usr/bin/php", argv, NULL);

    perror("execve");
    return ("");
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
