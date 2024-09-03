/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   WebservCleanUp.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: demre <demre@student.42malaga.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/01 20:06:23 by demre             #+#    #+#             */
/*   Updated: 2024/09/03 17:42:04 by demre            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Webserv.hpp"
#include "core.hpp"

void Webserv::checkTerminatedProcesses()
{
  int status;
  pid_t pid;

  // Use a non-blocking wait to clean up all finished child processes
  while ((pid = waitpid(-1, &status, WNOHANG)) > 0)
  {
    // Handle the case when a child process didn't terminate correctly
    if (!WIFEXITED(status) && !WIFSIGNALED(status))
    {
      std::cerr << "Child process " << pid << " didn't terminate correctly"
                << std::endl;

      // Find the pipe_fd from the pid, close the pipe and throw error
      for (std::map<int, pid_t>::iterator it = pidMap.begin();
           it != pidMap.end(); ++it)
      {
        if (it->second == pid)
        {
          size_t i = findClientIndexFromFD(it->first);
          pidMap.erase(it);
          closePipe(i);
          throw HttpException(500, "Child process didn't terminate correctly");
        }
      }
    }
    // Child process terminated successfully
    else if (WIFEXITED(status))
    {
      int exitStatus = WEXITSTATUS(status);
      for (std::map<int, pid_t>::iterator it = pidMap.begin();
           it != pidMap.end(); ++it)
      {
        if (it->second == pid)
        {
          // Remove the pid from the pidMap and add it to terminatedPidMap
          if (exitStatus == 0)
          {
            terminatedPidMap[it->first] = pid;
            pidMap.erase(it);
            break;
          }
          // Remove the pid from the pidMap and throw error
          else
          {
            pidMap.erase(it);
            throw HttpException(500, "Script exited with status: "
                                         + toString(exitStatus));
          }
        }
      }
    }
  }
}

// Close client connection and remove from pollfd and clients array, and remove any pending script pipes for that connection
void Webserv::closeConnection(size_t &i, const std::string &sessionId)
{
  std::cout << "Connection closed: " << fds[i].fd << std::endl;
  logConnectionCloseTime(sessionId);

  for (std::map<int, int>::const_iterator it = clientScriptMap.begin();
       it != clientScriptMap.end(); ++it)
  {
    if (it->second == fds[i].fd)
      clientScriptMap.erase(it->first);
  }
  close(fds[i].fd);
  fds.erase(fds.begin() + i);
  clients.erase(clients.begin() + i);
}

// Close pipe and remove from pollfd, clients and clientScriptMap array
void Webserv::closePipe(size_t &i)
{
  close(fds[i].fd);
  clientScriptMap.erase(fds[i].fd);
  fds.erase(fds.begin() + i);
  clients.erase(clients.begin() + i);
}
