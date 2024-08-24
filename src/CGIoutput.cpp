/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGIoutput.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blarger <blarger@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/10 14:38:48 by demre             #+#    #+#             */
/*   Updated: 2024/08/20 10:47:04 by blarger          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Webserv.hpp"
#include "core.hpp"

std::string Webserv::generateCgiOutputHtmlPage(std::string const &output,
                                               std::string const &URIpath)
{
  std::ostringstream htmlStream;

	std::cout << RED << "output = " << output << RESET << std::endl;
  htmlStream << "<html>\n"
             << "<head><title>CGI Script Output</title></head>\n"
             << "<body>\n"
             << "<h1>CGI script correctly executed</h1>\n"
             << "<h2>" << URIpath << "</h2>\n"
             << "<p>Content length is " << output.size() << "</p>\n"
             << "<p>CGI output: " << output << "</p>\n"
             << "</body>\n"
             << "</html>";

  return (htmlStream.str());
}

void Webserv::readAndHandleScriptOutput(size_t &i)
{
  size_t j = findClientIndexOfConnectionFromPipeFD(fds[i].fd);
  int clientFD = clients[j].socketFD;

  try
  {

    char buffer[4096];
    ssize_t bytesRead = read(fds[i].fd, buffer, sizeof(buffer) - 1);
    // std::cout << "A bytesRead: " << bytesRead << std::endl;
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
      j = findClientIndexFromFD(clientFD);
      clients[j].responseBuffer.clear();
      closeConnection(j);
      throw HttpException(500, "Error reading from pipe");
    }
    else if (bytesRead == 0)
    {
      // std::cout << "(bytesRead == 0)" << std::endl;
      std::string responseBody = generateCgiOutputHtmlPage(
          clients[j].responseBuffer, clients[j].req.URIpath);
      clients[j].response
          = composeOkHtmlResponse(responseBody, clients[j].req.buffer);
      clients[j].totalToSend = clients[j].response.size();
      clients[j].totalBytesSent = 0;
      clients[j].responseBuffer.clear();
      // Close pipe if child process has terminated
      if (terminatedPidMap.find(fds[i].fd) != terminatedPidMap.end())
      {
        terminatedPidMap.erase(fds[i].fd);
        closePipe(i);
        --i;
      }
    }
  }
  catch (const HttpException &e)
  {
    std::cerr << RED << "Error: " << e.getStatusCode() << " " << e.what()
              << RESET << '\n';

    j = findClientIndexFromFD(clientFD);
    clients[j].response = composeErrorHtmlPage(
        e.getStatusCode(), getReasonPhrase(e.getStatusCode()),
        clients[j].client_serverConfig.errorPages);
    clients[j].totalToSend = clients[j].response.size();
    clients[j].totalBytesSent = 0;
  }
}