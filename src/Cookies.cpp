/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cookies.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blarger <blarger@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/12 12:51:42 by blarger           #+#    #+#             */
/*   Updated: 2024/09/03 18:47:43 by blarger          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Cookies.hpp"
#include "Webserv.hpp"
#include "utils.hpp"

void logClientSessionRequest(ClientRequest &req)
{
  std::string filePath = std::string(LOG_DIR_PATH) + "cookies.log";

  std::ofstream outputFile(filePath.c_str(), std::ios::app);
  if (!outputFile)
  {
    std::cerr << "Error opening file to erase content: " << filePath
              << std::endl;
  }
  else
  {
    outputFile << req.sessionId << ", " << getLogDate() << ", " << req.method
               << " " << req.URIpath << std::endl;
    outputFile.close();
  }
}

std::string Webserv::getCookieRequestLine(const std::string &clientInput) const
{
  std::istringstream stream(clientInput);
  std::string line;

  while (std::getline(stream, line))
  {
    if (line.find("Cookie:") == 0)
      return (line);
  }

  return ("");
}

static std::string trimLastChar(std::string &str, char c)
{
  if (!str.empty() && str[str.size() - 1] == c)
  {
    return (str.substr(0, str.size() - 1));
  }
  return (str);
}

void Webserv::parseCookies(ClientRequest req)
{
  std::cout << YELLOW;
  std::string line;
  std::string key1;
  std::string key2;
  std::map<std::string, std::string> cookieMap;

  std::istringstream stream(getCookieRequestLine(req.buffer));
  std::getline(stream, line, ' '); //skip cookie
  std::cout << line;
  while (std::getline(stream, line, ';'))
  {
    std::istringstream lineStream(line);
    std::getline(lineStream, key1, '=');
    lineStream >> key2;
    key2 = trimLastChar(key2, ';');
    cookieMap[key1] = key2;
  }

  std::cout << std::endl << RESET;
}

std::string generateSessionID()
{
  static const char alphanum[] = "0123456789"
                                 "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
                                 "abcdefghijklmnopqrstuvwxyz";
  std::string sessionID;
  srand(time(0)); // Seed the random generator

  for (int i = 0; i < 32; ++i)
  { // Example: 32-character session ID
    sessionID += alphanum[rand() % (sizeof(alphanum) - 1)];
  }

  return (sessionID);
}

std::string findSessionID(std::string request)
{
  std::istringstream stream(request);
  std::string line;

  while (std::getline(stream, line))
  {
    if (line.find("Cookie: ") == 0)
    {
      size_t pos = line.find("sessionId=");
      if (pos != std::string::npos)
      {
        std::string sessionId = line.substr(pos + 10);
        std::istringstream _stream(sessionId);
        _stream >> sessionId;
        sessionId = trimLastChar(sessionId, ';');
        return (sessionId);
      }
      return ("");
    }
  }
  return ("");
}

void logConnectionCloseTime(const std::string &sessionId)
{
  std::string filePath = std::string(LOG_DIR_PATH) + "cookies.log";
  std::string time = getLogDate();

  std::ofstream outputFile(filePath.c_str(), std::ios::app);
  if (!outputFile)
  {
    std::cerr << "Error opening file to erase content: " << filePath
              << std::endl;
  }
  else
  {
    outputFile << sessionId << ", " << time << ", endConnection" << std::endl;
    outputFile.close();
  }
}

static void logConnectionStartTime(const std::string &sessionId)
{
  std::string filePath = std::string(LOG_DIR_PATH) + "cookies.log";
  std::cout << YELLOW << "sessionId : " << sessionId << RESET << std::endl;

  std::ofstream outputFile(filePath.c_str(),
                           std::ofstream::out | std::ofstream::app);
  if (!outputFile)
  {
    std::cerr << "Error opening file to write session data: " << filePath
              << std::endl;
  }
  else
  {
    std::string time = getLogDate();
    outputFile << sessionId << ", " << time << ", startConnection" << std::endl;
    outputFile.close();
  }
}

void saveSessionIdClient(std::map<std::string, SessionData> &sessions,
                          const std::string &reqBuffer,
                          ClientRequest &clientReq)
{
  std::string _sessionId = findSessionID(reqBuffer);

  if (_sessionId.empty())
  {
    clientReq.sessionId = generateSessionID();
    SessionData _session;
    _session.connectionStart = std::time(NULL);
    sessions[_sessionId] = _session;
    logConnectionStartTime(clientReq.sessionId);
  }
  else if (clientReq.sessionId != _sessionId)
  {
    clientReq.sessionId = _sessionId;
    SessionData _session;
    _session.connectionStart = std::time(NULL);
    sessions[_sessionId] = _session;
    logConnectionStartTime(_sessionId);
  }
}

std::string getCookieRequestLine(ClientRequest &clientReq)
{
  return ("Set-Cookie: sessionId=" + clientReq.sessionId + "; HttpOnly\r\n");
}