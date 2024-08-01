/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   WebservSignal.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blarger <blarger@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/01 11:37:18 by blarger           #+#    #+#             */
/*   Updated: 2024/08/01 12:20:14 by blarger          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Webserv.hpp"


Webserv* Webserv::instance = NULL;

void	Webserv::sigInt(int code)
{
	instance->handleSigInt(code);
}

void Webserv::handleSigInt(int code)
{
	std::cout << RED << "\n>>> SIGINT received [" << code << "]\n" << RESET;
	if (fds[0].fd != 0)
	{
		for (size_t i = 0; i < fds.size(); ++i) // server + incoming
		{
			std::cout << "Closing fd: " << fds[i].fd << std::endl;
			close(fds[i].fd);
		}		
	}
	exit(EXIT_SUCCESS);
}
