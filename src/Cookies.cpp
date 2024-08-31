/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cookies.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blarger <blarger@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/12 12:51:42 by blarger           #+#    #+#             */
/*   Updated: 2024/08/30 15:27:25 by blarger          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Webserv.hpp"

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

#include <string>

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
					while (std::getline(stream, line, '='))
					{
						if (line.find("sessionID") != std::string::npos)
						{
							stream >> line;
							line = trimLastChar(line, ';');
							return (line);
						}
						
					}
				}
    }
    return "";
}