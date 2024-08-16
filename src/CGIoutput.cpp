/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGIoutput.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: demre <demre@student.42malaga.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/10 14:38:48 by demre             #+#    #+#             */
/*   Updated: 2024/08/16 09:05:31 by demre            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Webserv.hpp"
#include "core.hpp"

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

void Webserv::handleScriptOutput(size_t &i)
{
  size_t j = findClientIndexFromPipeFD(fds[i].fd);
  int clientFD = clients[j].socketFD;

  try
  {

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
      clients[j].responseBuffer.clear();
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

    j = findClientIndexFromClientFD(clientFD);
    clients[j].response = composeErrorHtmlPage(
        e.getStatusCode(), getReasonPhrase(e.getStatusCode()),
        clients[j].client_serverConfig.errorPages);
  }
}