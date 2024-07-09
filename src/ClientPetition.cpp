/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ClientPetition.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blarger <blarger@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/09 11:49:01 by blarger           #+#    #+#             */
/*   Updated: 2024/07/09 13:24:48 by blarger          ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#include "ClientPetition.hpp"

void GET::findHeader(std::string &key, std::istringstream &isLine)
{

	if (key == "Host:")
	{
		isLine >> this->host;
		isLine >> key;
		findHeader(key, isLine);
	}
	else if (key == "User-Agent:")
	{
		isLine >> this->userAgent;
		isLine >> key;
		findHeader(key, isLine);
	}
	else if (key == "Accept:")
	{
		isLine >> this->accept;
		isLine >> key;
		findHeader(key, isLine);
	}
}

GET::GET(Webserv server, int serverFD, int clientFD, std::string clientInput)
{
	(void)server;
	(void)clientInput;
	(void)serverFD;
	(void)clientFD;

	std::istringstream isLine(clientInput);
	std::string key;

	std::cout << MAGENTA << clientInput << RESET << std::endl;
	isLine >> key;
	isLine >> this->pathToRessource;
	isLine >> this->HTTPversion;
	isLine >> key;
	// Read client input => Using flag ? using a "readline"
	/* if (key == "Host:" || key == "User-Agent:" || key == "Accept:")
		findHeader(key, isLine);
	std::cout << GREEN << "GET HTTML method instructions received!" << std::endl;
	write(clientFD, "GET client info\n", 14); */
	std::cout << RESET;
}

GET::GET(){};

GET::~GET(){};