/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   processMethods.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blarger <blarger@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/03 08:37:10 by blarger           #+#    #+#             */
/*   Updated: 2024/07/08 17:51:52 by blarger          ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#include "Webserv.hpp"

void processGetMethod(int serverFD, int clientFD)
{

	(void)serverFD;
	(void)clientFD;
	std::cout << GREEN << "GET HTTML method instructions received!" << std::endl;
	write(clientFD, "GET client info\n", 14);
	std::cout << RESET;
}

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

void processClientInput(const char *clientInput, int server_fd, int clientFD)
{
	if (clientInput[0] == 'G' && clientInput[1] == 'E' && clientInput[2] == 'T' && clientInput[3] == ' ')
		processGetMethod(server_fd, clientFD);
	else if (clientInput[0] == 'P' && clientInput[1] == 'U' && clientInput[2] == 'T' && clientInput[3] == ' ')
		processPutMethod(server_fd, clientFD);
	else if (clientInput[0] == 'D' && clientInput[1] == 'E' && clientInput[2] == 'L' && clientInput[3] == 'E' && clientInput[4] == 'T' && clientInput[5] == 'E' && clientInput[6] == ' ')
		processDeleteMethod(server_fd, clientFD);
	else
		std::cout << RED << "Unknown instruction received!" << RESET << std::endl;
}
