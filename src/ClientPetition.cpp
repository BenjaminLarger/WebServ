/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ClientPetition.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blarger <blarger@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/09 11:49:01 by blarger           #+#    #+#             */
/*   Updated: 2024/07/09 16:37:28 by blarger          ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#include "ClientPetition.hpp"

void GET::findHeader(std::string &key, std::istringstream &isLine)
{
	std::string newKey;
	std::cout << YELLOW << "key = " << key << RESET << std::endl;
	if (key == "Host:")
	{
		isLine >> this->host;
		std::cout << YELLOW << "Host = " << this->host << RESET << std::endl;
		isLine >> newKey;
		std::cout << YELLOW << "key = " << key << RESET << std::endl;
		if (key != newKey)
			findHeader(newKey, isLine);
	}
	else if (key == "User-Agent:")
	{
		isLine >> this->userAgent;
		std::cout << YELLOW << "userAgent = " << this->host << RESET << std::endl;
		isLine >> newKey;
		std::cout << YELLOW << "key = " << key << RESET << std::endl;
		if (key != newKey)
			findHeader(newKey, isLine);
	}
	else if (key == "Accept:")
	{
		isLine >> this->accept;
		std::cout << YELLOW << "accept = " << this->accept << RESET << std::endl;
		isLine >> newKey;
		std::cout << YELLOW << "key = " << key << RESET << std::endl;
		if (key != newKey)
			findHeader(newKey, isLine);
	}
}

int countJumpLine(std::string str)
{
	int count = 0;

	for (int i = 0; str[i]; i++)
	{
		if (str[i] == '\n')
			count++;
	}
	return (count);
}

GET::GET(Webserv server, int serverFD, int clientFD, std::string &clientInput)
{
	(void)server;
	(void)clientInput;
	(void)serverFD;
	(void)clientFD;

	if (countJumpLine(clientInput) != 3)
		return;
	std::istringstream isLine(clientInput);
	std::string key;

	std::cout << MAGENTA << clientInput << RESET << std::endl;
	isLine >> key;
	std::cout << BLUE << "key = " << key << RESET << std::endl;
	isLine >> this->pathToRessource;
	isLine >> this->HTTPversion;
	isLine >> key; // Host
	std::cout << BLUE << "key = " << key << RESET << std::endl;

	// Read client input => Using flag ? using a "readline"
	if (key == "Host:" || key == "User-Agent:" || key == "Accept:")
		findHeader(key, isLine);
	std::cout << "host = " << this->host << std::endl;
	write(clientFD, "GET client info\n", 14);
	clientInput.erase();
	std::cout << RESET;
}

GET::GET(){};

GET::~GET(){};