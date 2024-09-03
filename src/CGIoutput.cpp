/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGIoutput.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blarger <blarger@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/10 14:38:48 by demre             #+#    #+#             */
/*   Updated: 2024/09/03 18:04:15 by blarger          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Webserv.hpp"
#include "core.hpp"

static std::string getContentType(const std::string &response)
{
  size_t pos = response.find("Content-Type:");
  if (pos != std::string::npos)
  {
    size_t start = pos + 13;
    size_t end = response.find("\n", start);
    std::string contentType = response.substr(start, end - start);
    trimBothEnds(contentType);
    return (contentType);
  }
  return ("");
}

std::string Webserv::generateCgiOutputHtmlPage(std::string const &output,
                                               std::string const &URIpath)
{
  std::ostringstream htmlStream;

  htmlStream << "<html>\n"
             << "<head><title>CGI Script Output</title>\n"
             << extractHtmlContentFromFile("./var/www/html/date_style.html")
             << "</head>\n"
             << "<body>\n"
             << "<h1>CGI script correctly executed</h1>\n"
             << "<h2>" << URIpath << "</h2>\n"
             << "<p>Content length is " << output.size() << "</p>\n"
             << "<p>CGI output: " << output << "</p>\n"
             << "<br>\n"
             << "</body>\n"
             << "</html>\n";

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
    if (bytesRead > 0)
    {
      buffer[bytesRead] = '\0';
      clients[j].responseBuffer += buffer;
    }
    else if (bytesRead < 0)
    {
      closePipe(i);
      --i;
      j = findClientIndexFromFD(clientFD);
      clients[j].responseBuffer.clear();
      closeConnection(j, clients[i].req.sessionId);
    }
    else if (bytesRead == 0)
    {
      // Check for terminated child processes
      checkTerminatedProcesses();

      // Close pipe if child process has terminated
      if (terminatedPidMap.find(fds[i].fd) != terminatedPidMap.end())
      {
        {
          std::string contentType = getContentType(clients[j].responseBuffer);
          // If not html, build html page and add headers
          if (contentType != "text/html")
          {
            std::string responseBody = generateCgiOutputHtmlPage(
                clients[j].responseBuffer, clients[j].req.URIpath);
            clients[j].response
                = composeOkHtmlResponse(responseBody, clients[j].req);
          }
          // If cgi output is already html, insert the output string responseBuffer into response char vector
          else
          {
            clients[j].response.insert(clients[j].response.begin(),
                                       clients[j].responseBuffer.begin(),
                                       clients[j].responseBuffer.end());
          }

          clients[j].totalToSend = clients[j].response.size();
          clients[j].totalBytesSent = 0;
          clients[j].responseBuffer.clear();
        }

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