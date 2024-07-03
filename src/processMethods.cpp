/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   processMethods.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blarger <blarger@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/03 08:37:10 by blarger           #+#    #+#             */
/*   Updated: 2024/07/03 11:50:13 by blarger          ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#include "Webserv.hpp"

void	processGetMethod(int serverFD, int clientFD)
{
	
	(void)serverFD;
	(void)clientFD;
	std::cout << GREEN << "GET HTTML method instructions received!" << std::endl;
	write(clientFD, "GET client info\n", 14);
	std::cout << RESET;

}

void	processPutMethod(int serverFD, int clientFD)
{
	
	(void)serverFD;
	(void)clientFD;
	std::cout << GREEN << "PUT HTTML method instructions received!" << RESET << std::endl;

}

void	processDeleteMethod(int serverFD, int clientFD)
{
	
	(void)serverFD;
	(void)clientFD;
	std::cout << GREEN << "DELETE HTTML method instructions received!" << RESET << std::endl;

}

void	processClientInput(const char *clientInput, int server_fd, int clientFD)
{
	if (!strncmp(clientInput, "GET", 3) && clientInput[3] == ' ')
		processGetMethod(server_fd, clientFD);
	else if (!strncmp(clientInput, "PUT", 3) && clientInput[3] == ' ')
		processPutMethod(server_fd, clientFD);
	else if (!strncmp(clientInput, "DELETE", 6) && clientInput[6] == ' ')
		processDeleteMethod(server_fd, clientFD);
	else
		std::cout << RED << "Unknown instruction received!" << RESET << std::endl;
}