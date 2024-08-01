/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ManageWriteReadClientCapability.cpp                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blarger <blarger@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/01 13:20:00 by blarger           #+#    #+#             */
/*   Updated: 2024/08/01 16:02:36 by blarger          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Webserv.hpp"

// Function to withdraw read capability
void Webserv::withdrawReadCapability(size_t clientIndex, std::string &buffer)
{
	std::cout << RED << "Read capability withdrawn\n" << RESET;
	std::cout << "event = " << fds[clientIndex].events << ", POLLIN = " << POLLIN << RESET << std::endl;
    fds[clientIndex].events &= ~POLLIN; // Remove POLLIN flag
	std::cout << "event = " << fds[clientIndex].events << ", POLLIN = " << POLLIN << RESET << std::endl;
	buffer.erase();
}

// Function to withdraw write capability
void Webserv::withdrawWriteCapability(size_t clientIndex, std::string &buffer)
{
	std::cout << RED << "Read capability withdrawn\n" << RESET;
    fds[clientIndex].events &= ~POLLOUT; // Remove POLLOUT flag
	buffer.erase();
}

// Function to restore read capability
void Webserv::restoreReadCapability(size_t clientIndex, std::string &buffer)
{
	std::cout << GREEN << "Read capability restored\n" << RESET;
	std::cout << "event = " << fds[clientIndex].events << ", POLLIN = " << POLLIN << RESET << std::endl;
    fds[clientIndex].events |= POLLIN; // Add POLLIN flag
	std::cout << "event = " << fds[clientIndex].events << ", POLLIN = " << POLLIN << RESET << std::endl;
	buffer.erase();
}

// Function to restore write capability
void Webserv::restoreWriteCapability(size_t clientIndex, std::string &buffer)
{
	std::cout << GREEN << "Write capability restored\n" << RESET;
    fds[clientIndex].events |= POLLOUT; // Add POLLOUT flag
	buffer.erase();
}