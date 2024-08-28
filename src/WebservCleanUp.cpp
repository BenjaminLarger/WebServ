/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   WebservCleanUp.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blarger <blarger@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/01 20:06:23 by demre             #+#    #+#             */
/*   Updated: 2024/08/24 18:32:46 by blarger          ###   ########.fr       */
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
		std::cout << "Wait done\n";
    // Handle the case when a child process didn't terminate correctly
    if (!WIFEXITED(status) && !WIFSIGNALED(status))
    {
      std::cerr << "Child process " << pid << " didn't terminate correctly"
                << std::endl;

      // Find the pipe_fd from the pid, close the pipe and remove the client connection
      for (std::map<int, pid_t>::iterator it = pidMap.begin();
           it != pidMap.end(); ++it)
      {
        if (it->second == pid)
        {
          size_t i = findClientIndexFromFD(it->first);
          int clientFD = clientScriptMap[it->first];
          pidMap.erase(it);
          closePipe(i);
          size_t j = findClientIndexFromFD(clientFD);
          closeConnection(j);
          break;
        }
      }
    }
    else
    {
      // Remove the pid from the pidMap and add it to terminatedPidMap
			std::cout << "wait else\n" << std::endl;
      for (std::map<int, pid_t>::iterator it = pidMap.begin();
           it != pidMap.end(); ++it)
      {
				std::cout << "it->second" << it->second << std::endl;
        if (it->second == pid)
        {
					std::cout << "it->second == pid\n";
          terminatedPidMap[it->first] = pid;
          pidMap.erase(it);
          break;
        }
      }
    }
  }
}

// Close client connection and remove from pollfd and clients array, and remove any pending script pipes for that connection
void Webserv::closeConnection(size_t &i)
{
  std::cout << "Connection closed: " << fds[i].fd << std::endl;

  std::cout << "size() fds: " << fds.size() << ", clients: " << clients.size()
            << ", clientScriptMap: " << clientScriptMap.size() << std::endl;

  for (std::map<int, int>::const_iterator it = clientScriptMap.begin();
       it != clientScriptMap.end(); ++it)
  {
    if (it->second == fds[i].fd)
      clientScriptMap.erase(it->first);
  }
  close(fds[i].fd);
  fds.erase(fds.begin() + i);
  clients.erase(clients.begin() + i);

  std::cout << "size() fds: " << fds.size() << ", clients: " << clients.size()
            << ", clientScriptMap: " << clientScriptMap.size() << std::endl;
}

// Close pipe and remove from pollfd, clients and clientScriptMap array
void Webserv::closePipe(size_t &i)
{
  std::cout << "Pipe closed: " << fds[i].fd << std::endl;

  std::cout << "size() fds: " << fds.size() << ", clients: " << clients.size()
            << ", clientScriptMap: " << clientScriptMap.size() << std::endl;

  close(fds[i].fd);
  clientScriptMap.erase(fds[i].fd);
  fds.erase(fds.begin() + i);
  clients.erase(clients.begin() + i);

  std::cout << "size() fds: " << fds.size() << ", clients: " << clients.size()
            << ", clientScriptMap: " << clientScriptMap.size() << std::endl;
}