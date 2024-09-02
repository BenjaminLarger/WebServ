/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cookies.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blarger <blarger@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/12 12:51:42 by blarger           #+#    #+#             */
/*   Updated: 2024/09/02 20:00:27 by blarger          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Webserv.hpp"
#include "Cookies.hpp"

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
							return (sessionId);
					}
					return ("");
				}
    }
    return "";
}

bool addLogoutTimeCookies(std::string filePath, std::map<std::string, SessionData> &sessions, const std::string &sessionId)
{
	std::ifstream inputFile(filePath.c_str());
	sessions[sessionId].connectionEnd = std::time(NULL);

	if (!inputFile)
	{
			std::cerr << RED << RED << "Error opening file for reading: " << filePath << RESET << RESET<< std::endl;
			return (false);
	}

	std::string tempFilePath = filePath + ".tmp";
	std::ofstream tempFile(tempFilePath.c_str());
	if (!tempFile)
	{
			std::cerr << RED << RED << "Error opening temporary file for writing: " << tempFilePath << RESET << RESET<< std::endl;
			return (false);
	}

	std::string line;
	bool sessionIdFound = (false);
	while (std::getline(inputFile, line))
	{
		if (line.find("Session ID: " + sessionId) != std::string::npos)
		{
				sessionIdFound = true;
				char buffer[80];
				struct tm* timeinfo = localtime(&sessions[sessionId].connectionEnd);
				strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", timeinfo);
				line += ", End of session: " + std::string(buffer);
		}
		tempFile << line << std::endl;
	}

	inputFile.close();
  tempFile.close();

	if (!sessionIdFound)
	{
			std::cerr << RED << "sessionId " << sessionId << "not found in the file: " << sessionId << RESET<< std::endl;
			std::remove(tempFilePath.c_str());
			return (false);
	}

	if (std::rename(tempFilePath.c_str(), filePath.c_str()) != 0) {
			std::cerr << RED << "Error replacing the original file with the temporary file" << RESET<< std::endl;
			return (false);
	}
	std::remove(tempFilePath.c_str());
	return (true);
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

    // Open the file in append mode
    std::ofstream ofs(filePath.c_str(), std::ofstream::out | std::ofstream::app);
    if (!ofs)
		{
        std::cerr << "Error opening file to write session data: " << filePath << std::endl;
        return;
    }

		if (isSessionIdPresent(filePath, sessionId) == false)
		{
			// Convert connection start time to a human-readable format
			char buffer[80];
			struct tm* timeinfo = localtime(&session.connectionStart);
			strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", timeinfo);
			// Write sessionId and connection start time to the file
			ofs << "Session ID: " << sessionId << ", Connection Start: " << buffer << std::endl;
		}
    // Close the file
    ofs.close();
}

std::string	handleCookiesSessions(std::map<std::string, SessionData> &sessions,
																	const std::string &reqBuffer,
																	ClientRequest &clientReq)
{
	std::string sessionIdLine;
	std::string sessionId = findSessionID(reqBuffer);
  size_t findSessionId = reqBuffer.find("sessionId");

  if (findSessionId != std::string::npos && reqBuffer[findSessionId + 10] != 13)
  {
    sessionIdLine = "Set-Cookie: sessionId=" + sessionId
                + "; HttpOnly\r\n";
		std::cout << GREEN << "Session ID found: " << sessionId << RESET << std::endl;
		if (isSessionIdPresent(std::string(LOG_DIR_PATH) + "cookies.log", sessionId) == false)
		{
				SessionData _session;
				_session.connectionStart = std::time(NULL);
				sessions[sessionId] = _session;
		}
  }
  else
  {
		sessionId = generateSessionID();
    sessionIdLine
        = "Set-Cookie: sessionId=" + sessionId + "; HttpOnly\r\n";
    std::cout << GREEN
              << "session ID not found in request => generating new one : "
							<< sessionIdLine
              << RESET << std::endl;
				SessionData session;
				session.connectionStart = std::time(NULL);
				sessions[sessionId] = session;
  }
	clientReq.sessionId = sessionId;
	logSessionData(sessions[sessionId], sessionId);
	return (sessionIdLine);
}