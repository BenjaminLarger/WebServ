/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blarger <blarger@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/07 09:46:47 by blarger           #+#    #+#             */
/*   Updated: 2024/07/03 11:50:15 by blarger          ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#include "Webserv.hpp"


#define PORT 8081
#define MAX_CLIENTS 100

int set_nonblocking(int fd)
{
    int flags = fcntl(fd, F_GETFL, 0);
    if (flags < 0) return -1;
    return fcntl(fd, F_SETFL, flags | O_NONBLOCK);
}

void serverListeningLoop(int server_fd)
{
	std::vector<pollfd> fds;
    struct pollfd temp_fd;
	temp_fd.fd = server_fd;
	temp_fd.events = POLLIN;
	temp_fd.revents = 0;
	fds.push_back(temp_fd);

    while (true)
	{
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
                    while (true)
					{
                        int new_socket = accept(server_fd, NULL, NULL);
                        if (new_socket < 0) {
                            if (errno == EWOULDBLOCK || errno == EAGAIN)
							{
                                // No more incoming connections
                                break;
                            }
							else
							{
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
                }
				else
				{
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
                       // write(fds[i].fd, buffer, bytes_read);
						processClientInput(buffer, server_fd, fds[i].fd);
                    }
                }
            }
        }
    }
}

int main(int argc, char **argv)
{
	try
	{
		(void)argv;
		if (argc != 2)
			throw (std::out_of_range(ARG));

		Webserv	start(8080);
	}
	catch(const std::exception& e)
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