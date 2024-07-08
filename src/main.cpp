/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blarger <blarger@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/07 09:46:47 by blarger           #+#    #+#             */
/*   Updated: 2024/07/08 17:26:42 by blarger          ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#include "Webserv.hpp"

#define PORT 8081
#define MAX_CLIENTS 100

void serverListeningLoop(int serverFD)
{
	std::vector<pollfd> fds = initializePollFDSWithServerSocket(serverFD);

	while (true)
	{
		monitorSocketEvents(fds, serverFD);
		for (size_t i = 0; i < fds.size(); ++i)
		{
			// evaluates to true if the i-th file descriptor has incoming data available for reading.
			if (fds[i].revents & POLLIN)
			{
				if (fds[i].fd == serverFD)
				{
					// Accept new connections
					while (true)
					{
						int newSocket = accept(serverFD, NULL, NULL);
						if (newSocket < 0)
						{
							if (errno == EWOULDBLOCK || errno == EAGAIN)
							{
								// No more incoming connections
								break;
							}
							else
							{
								close(serverFD);
								throw(std::runtime_error("accept failed"));
							}
						}
						std::cout << "New connection accepted: " << newSocket << std::endl;
						setNonBlocking(newSocket);
						struct pollfd newTempFD = setNewTempFDStruct(newSocket);
						fds.push_back(newTempFD);
					}
				}
				else
				{
					// Handle data from an existing connection
					processConnectionData(serverFD, fds, i);
				}
			}
		}
	}
}

int main(int argc, char **argv)
{
	ServerConfig file;

	try
	{
		(void)argv;
		if (argc != 2)
			throw(std::out_of_range(ARG));

		// file = file.parsConfigFile(argv[1]);
		Webserv start(8080, argv[1]);
	}
	catch (const std::exception &e)
	{
		std::cerr << RED << "ERROR: " << RESET << e.what() << std::endl;
		return (-1);
	}
	catch (...)
	{
		std::cerr << RED << "Unknown error!" << RESET << std::endl;
		return (-1);
	}

	return 0;
}