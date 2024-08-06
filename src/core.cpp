/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   core.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: demre <demre@student.42malaga.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/05 19:51:10 by demre             #+#    #+#             */
/*   Updated: 2024/08/06 16:30:25 by demre            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "core.hpp"

int sendall(int s, const char *buf, int len)
{
  int totalBytesSent = 0; // how many bytes we've sent
  int bytesleft = len;    // how many we have left to send
  int n;

  while (totalBytesSent < len)
  {
    n = send(s, buf + totalBytesSent, bytesleft, 0);
    if (n == -1)
      break;
    totalBytesSent += n;
    bytesleft -= n;
  }

  len = totalBytesSent; // return number actually sent here

  return (n == -1 ? -1 : 0); // return -1 on failure, 0 on success
}

bool isDirectory(const std::string &path)
{
  struct stat path_stat;
  if (stat(path.c_str(), &path_stat) != 0)
  {
    return (false);
  }
  return (S_ISDIR(path_stat.st_mode));
}
