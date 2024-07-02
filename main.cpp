/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blarger <blarger@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/07 09:46:47 by blarger           #+#    #+#             */
/*   Updated: 2024/07/02 18:19:11 by blarger          ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#include "Webserv.hpp"
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <fcntl.h>
#include <poll.h>
#include <iostream>
#include <cerrno>
#include <cmath>
#include <cstdio>


#include <poll.h>
#include <iostream>
#include <cerrno>
#include <cmath>
#include <cstdio>
#include <cstring>
#include <vector>

#define PORT 8081
#define MAX_CLIENTS 100

int set_nonblocking(int fd) {
    int flags = fcntl(fd, F_GETFL, 0);
    if (flags < 0) return -1;
    return fcntl(fd, F_SETFL, flags | O_NONBLOCK);
}

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

void serverListeningLoop(int server_fd)
{
	std::vector<pollfd> fds;
    struct pollfd temp_fd;
	temp_fd.fd = server_fd;
	temp_fd.events = POLLIN;
	temp_fd.revents = 0;
	fds.push_back(temp_fd);

    while (true) {
        int poll_count = poll(fds.data(), fds.size(), -1);
        if (poll_count < 0) {
            perror("poll failed");
            close(server_fd);
            exit(EXIT_FAILURE);
        }
        for (size_t i = 0; i < fds.size(); ++i) {
            if (fds[i].revents & POLLIN) {
                if (fds[i].fd == server_fd) {
                    // Accept new connections
                    while (true) {
                        int new_socket = accept(server_fd, NULL, NULL);
                        if (new_socket < 0) {
                            if (errno == EWOULDBLOCK || errno == EAGAIN) {
                                // No more incoming connections
                                break;
                            } else {
                                perror("accept failed");
                                close(server_fd);
                                exit(EXIT_FAILURE);
                            }
                        }
                        std::cout << "New connection accepted: " << new_socket << std::endl;
                        set_nonblocking(new_socket);
                        struct pollfd new_temp_fd;
						new_temp_fd.fd = new_socket;
						new_temp_fd.events = POLLIN;
						new_temp_fd.revents = 0;
						fds.push_back(new_temp_fd);
                    }
                } else {
                    // Handle data from an existing connection
                    char buffer[1024];
                    memset(buffer, 0, sizeof(buffer));
                    int bytes_read = read(fds[i].fd, buffer, sizeof(buffer));
                    if (bytes_read <= 0) {
                        if (bytes_read == 0) {
                            std::cout << "Connection closed: " << fds[i].fd << std::endl;
                        } else {
                            perror("read failed");
                        }
                        close(fds[i].fd);
                        fds.erase(fds.begin() + i);
                        --i;
                    } else
					{
                        std::cout << "Received data: " << buffer << std::endl;
                        // Echo the data back to the client
                        write(fds[i].fd, buffer, bytes_read);
						processClientInput(buffer, server_fd, fds[i].fd);
                    }
                }
            }
        }
    }
}

int main() {
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // Set the socket to non-blocking
    if (set_nonblocking(server_fd) < 0) {
        perror("set_nonblocking failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    struct sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr*)&address, sizeof(address)) < 0) {
        perror("bind failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    if (listen(server_fd, 10) < 0) {
        perror("listen failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    std::vector<pollfd> fds;
    struct pollfd temp_fd;
	temp_fd.fd = server_fd;
	temp_fd.events = POLLIN;
	temp_fd.revents = 0;
	fds.push_back(temp_fd);

   serverListeningLoop(server_fd);

    close(server_fd);
    return 0;
}