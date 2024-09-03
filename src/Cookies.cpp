/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cookies.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blarger <blarger@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/12 12:51:42 by blarger           #+#    #+#             */
/*   Updated: 2024/09/03 16:53:14 by blarger          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Webserv.hpp"
#include "Cookies.hpp"


void	logClientSessionRequest(ClientRequest &req)
{
	std::string	filePath = std::string(LOG_DIR_PATH) + "cookies.log";

	std::ofstream outputFile(filePath.c_str(), std::ios::app);
	if (!outputFile)
	{
			std::cerr << "Error opening file to erase content: " << filePath << std::endl;
	}
	else
	{
			 outputFile << "Session ID: " << req.sessionId
			 << ", URI consulted: " << req.URIpath
			 << std::endl;
	}
}

					
std::string Webserv::getCookieLine(const std::string &clientInput) const
{
    std::istringstream stream(clientInput);
    std::string line;

    while (std::getline(stream, line))
		{
        if (line.find("Cookie:") == 0)
            return line;
    }

    return "";
}

std::string trimLastChar(std::string &str, char c)
{
    if (!str.empty() && str[str.size() - 1] == c) {
        return (str.substr(0, str.size() - 1));
    }
    return (str);
}

void	Webserv::parseCookies(ClientRequest req)
{
	std::cout << YELLOW;
	std::string	line;
	std::string	key1;
	std::string	key2;
	std::map<std::string, std::string> cookieMap;

	std::istringstream stream(getCookieLine(req.buffer));
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
    static const char alphanum[] =
        "0123456789"
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

std::string	findSessionID(std::string request)
{
	std::istringstream stream(request);
  std::string line;

    while (std::getline(stream, line))
		{
        if (line.find("Cookie: ") == 0)
        {
					std::cout << YELLOW << "line : " << line << RESET << std::endl;
					size_t pos = line.find("sessionId=");
					if (pos != std::string::npos)
					{
							std::string sessionId = line.substr(pos + 10);
							sessionId = trimLastChar(sessionId, 13);
							sessionId = trimLastChar(sessionId, ';');
							std::cout << ORANGE << "sessionId = " << sessionId << RESET << std::endl;
							std::istringstream _stream(sessionId);
							_stream >> sessionId;
							return (sessionId);
					}
					return ("");
				}
    }
    return "";
}

void addLogoutTimeCookies(std::string filePath, std::map<std::string, SessionData> &sessions, const std::string &sessionId)
{
	char buffer[80];
	sessions[sessionId].connectionEnd = std::time(NULL);
	struct tm* timeinfo = localtime(&sessions[sessionId].connectionEnd);
	strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", timeinfo);

	std::ofstream outputFile(filePath.c_str(), std::ios::app);
	if (!outputFile)
	{
			std::cerr << "Error opening file to erase content: " << filePath << std::endl;
	}
	else
	{
			 outputFile << "Session ID: " << sessionId
			 << ", End of session: " << buffer
			 << std::endl;
	}
}

bool isSessionIdPresent(const std::string& filePath, const std::string& sessionId)
{
    std::ifstream ifs(filePath.c_str());
    if (!ifs)
		{
        std::cerr << "Error opening file to read session data: " << filePath << std::endl;
        return (false);
    }

    std::string line;
    while (std::getline(ifs, line))
		{
        if (line.find("Session ID: " + sessionId) != std::string::npos)
            return (true);
    }

    return (false);
}

void logSessionData(const SessionData& session, const std::string &sessionId)
{
    // Construct the file path
    std::string filePath = std::string(LOG_DIR_PATH) + "cookies.log";
		std::cout << YELLOW <<"sessionId : " << sessionId << RESET << std::endl;
    // Open the file in append mode
    std::ofstream ofs(filePath.c_str(), std::ofstream::out | std::ofstream::app);
    if (!ofs)
		{
        std::cerr << "Error opening file to write session data: " << filePath << std::endl;
        return;
    }

		// Convert connection start time to a human-readable format
		char buffer[80];
		struct tm* timeinfo = localtime(&session.connectionStart);
		strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", timeinfo);
		// Write sessionId and connection start time to the file
		ofs << "Session ID: " << sessionId << ", Connection Start: " << buffer << std::endl;
    // Close the file
    ofs.close();
}

void	checkSessionIdClient(std::map<std::string, SessionData> &sessions,
																	const std::string &reqBuffer,
																	ClientRequest &clientReq)
{
	std::string	_sessionId = findSessionID(reqBuffer);

	if (_sessionId.empty() /* && clientReq.sessionId.empty() */)
	{
		clientReq.sessionId = generateSessionID();
		std::cout << GREEN
              << "Generating new session ID : "
							<< clientReq.sessionId
              << RESET << std::endl;
		SessionData _session;
		_session.connectionStart = std::time(NULL);
		sessions[_sessionId] = _session;
		logSessionData(sessions[_sessionId], clientReq.sessionId);
	}
	else if (clientReq.sessionId != _sessionId)
	{
		std::cout << GREEN
              << "Attributing : "
							<< clientReq.sessionId
							<< " to clientReq.sessionId"
              << RESET << std::endl;
		clientReq.sessionId = _sessionId;
		SessionData _session;
		_session.connectionStart = std::time(NULL);
		sessions[_sessionId] = _session;
		logSessionData(sessions[_sessionId], _sessionId);
	}
	else
		std::cout << ORANGE << "Client has already a sessionId: " << clientReq.sessionId << RESET << std::endl;
}

std::string	handleCookiesSessions(std::map<std::string, SessionData> &sessions,
																	const std::string &reqBuffer,
																	ClientRequest &clientReq)
{
	std::string sessionIdLine;
	(void)sessions;
	(void)reqBuffer;

  sessionIdLine
        = "Set-Cookie: sessionId=" + clientReq.sessionId + "; HttpOnly\r\n";

	return (sessionIdLine);
}