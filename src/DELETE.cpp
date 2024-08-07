/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   DELETE.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isporras <isporras@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/07 15:57:35 by isporras          #+#    #+#             */
/*   Updated: 2024/08/07 17:23:20 by isporras         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "DELETE.hpp"
#include "HttpExceptions.hpp"

void DELETE::setHost(const std::string &_host) { this->host = _host; }
void DELETE::setAuthorization(const std::string &_authorization) { this->authorization = _authorization; }
void DELETE::setIfMatch(const std::string &_if_match) { this->if_match = _if_match; }
std::string DELETE::getHost(void) const { return this->host; }
std::string DELETE::getAuthorization(void) const { return this->authorization; }
std::string DELETE::getIfMatch(void) const { return this->if_match; }

void DELETE::checkPreconditions(std::string expectedEtag, std::string expectedAuth)
{
	if (expectedEtag != getIfMatch())
		throw HttpException("412", "Precondition Failed: ETag does not match");
	if (expectedAuth != getAuthorization())
		throw HttpException("401", "Unauthorized");
}

void DELETE::findHeader(std::string &key, std::istringstream &isLine)
{
	std::string newKey;
	typedef void (DELETE::*HeaderSetter)(const std::string &);
	std::map<std::string, HeaderSetter> headersMap;

	headersMap["Host:"] = &DELETE::setHost;
	headersMap["Authorization:"] = &DELETE::setAuthorization;
	headersMap["If-Match:"] = &DELETE::setIfMatch;

	std::map<std::string, HeaderSetter>::iterator it = headersMap.begin();

	while (it != headersMap.end())
	{ // Loop through all headers
		if (it->first == key)
		{
			std::cout << GREEN << "Header found ! " << key << std::endl;
			isLine >> newKey;
			std::cout << ORANGE << "new key = " << newKey << std::endl;
			while (isLine.peek() != '\n' && isLine.peek() != 13
					&& isLine.peek() != EOF)
			{
				std::string temp;
				isLine >> temp;
				newKey += temp;
			}
			std::cout << RED << "new key = " << newKey << std::endl;
			if (key != newKey)
			{
				//We use the link to the map to call the function that will set the value of the object
				(this->*(it->second))(newKey);
				isLine >> newKey;
				findHeader(newKey, isLine);
			}
		}
		++it;
	}
}

void DELETE::parseRequest(std::string &clientInput)
{
	std::cout << MAGENTA << clientInput << RESET << std::endl;
	std::istringstream isLine(clientInput);
	std::string key;

	isLine >> key;
	isLine >> pathToRessource;
	isLine >> HTTPversion;
	// START PARSING HEADER
	isLine >> key;
	findHeader(key, isLine);
}

DELETE::DELETE(int clientFD, std::string &clientInput, const ServerConfig &serverConfig)
	: serverConfig(serverConfig)
{
	try
	{
		parseRequest(clientInput);
		// Example of tag. Then we maybe need to create them dynamicaly
		std::string expectedEtag = "\"e0023aa4e\""; 
		std::string expectedAuth = "anytoken";
		checkPreconditions(expectedEtag, expectedAuth);
		if (remove(pathToRessource.c_str()) != 0)
			throw HttpException("500", "Internal Server Error");
		//sendResponse
	}
	catch (const HttpException &e)
	{
		std::cerr << RED << "Error: " << e.what() << RESET << '\n';
		sendDefaultErrorPage(clientFD, e.getStatusCode(), e.getErrorMessage(),
                         serverConfig.errorPages);
	}
}