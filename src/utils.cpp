/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: demre <demre@student.42malaga.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/31 15:30:21 by blarger           #+#    #+#             */
/*   Updated: 2024/08/02 12:43:03 by demre            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Dependencies.hpp"

int sendall(int s, const char *buf, int len)
{
  int totalBytesSent = 0; // how many bytes we've sent
  int bytesleft = len;    // how many we have left to send
  int n;

  while (totalBytesSent < len)
  {
    n = send(s, buf + totalBytesSent, bytesleft, 0);
    if (n == -1)
    {
      break;
    }
    totalBytesSent += n;
    bytesleft -= n;
  }

  len = totalBytesSent; // return number actually sent here

  return (n == -1 ? -1 : 0); // return -1 on failure, 0 on success
}

bool isAllWhitespace(const std::string &str)
{
  for (std::string::const_iterator it = str.begin(); it != str.end(); ++it)
  {
    if (!std::isspace(*it))
      return (false);
  }
  return (true);
}

std::string intToString(int value)
{
  std::ostringstream oss;
  oss << value;
  return (oss.str());
}
