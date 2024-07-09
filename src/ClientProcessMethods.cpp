/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ClientProcessMethods.cpp                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blarger <blarger@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/03 08:37:10 by blarger           #+#    #+#             */
/*   Updated: 2024/07/09 17:05:08 by blarger          ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#include "ClientPetition.hpp"

void processPutMethod(int serverFD, int clientFD)
{

	(void)serverFD;
	(void)clientFD;
	std::cout << GREEN << "PUT HTTML method instructions received!" << RESET << std::endl;
	write(clientFD, "PUT client info\n", 14);
}

void processDeleteMethod(int serverFD, int clientFD)
{

	(void)serverFD;
	(void)clientFD;
	std::cout << GREEN << "DELETE HTTML method instructions received!" << RESET << std::endl;
	write(clientFD, "DELETE client info\n", 17);
}

void Webserv::processClientInput(std::string clientInput, int serverFD, int clientFD, std::string &staticBuffer)
{
	staticBuffer += clientInput;

	if (staticBuffer[0] == 'G' && staticBuffer[1] == 'E' && staticBuffer[2] == 'T' && staticBuffer[3] == ' ')
	{
		// processGetMethod(serverFD, clientFD);
		GET method(*this, serverFD, clientFD, staticBuffer);
	}
	else if (staticBuffer[0] == 'P' && staticBuffer[1] == 'U' && staticBuffer[2] == 'T' && staticBuffer[3] == ' ')
		processPutMethod(serverFD, clientFD);
	else if (staticBuffer[0] == 'D' && staticBuffer[1] == 'E' && staticBuffer[2] == 'L' && clientInput[3] == 'E' && clientInput[4] == 'T' && clientInput[5] == 'E' && clientInput[6] == ' ')
		processDeleteMethod(serverFD, clientFD);
	else
	{
		staticBuffer.erase();
		std::cout << RED << "Unknown instruction received!" << " staticBuffer = " << staticBuffer << RESET << std::endl;
	}
}
