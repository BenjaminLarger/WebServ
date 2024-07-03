/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigurationFile.hpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blarger <blarger@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/03 11:23:15 by blarger           #+#    #+#             */
/*   Updated: 2024/07/03 12:40:45 by blarger          ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#pragma once

#ifndef WEBSERV_HPP
#define WEBSERV_HPP

#include "Dependcies.hpp"

// ************************************************************************** //
//                               Class                                		  //
// ************************************************************************** //


class LocationConfig
{

	private:
		std::string					root;
		int							index;
		std::vector<std::string>	allowedMethods;
			
	public:
/* 		class InvalidNameForm : public std::exception
		{
			const char* what() const throw()
			{
				return ("The name form is unknown.");
			}
		}; */
};

class ServerConfig {

private:
	unsigned int				port;
	std::string					serverName;
	std::map<int, std::string>	errorPages;
	std::string					host;
	unsigned int				maxBodySize;

public:
	void	parsConfigFile(char *filename);

	const int			&getServerFD(void) const;
	const unsigned int	&getPort(void) const;
};

#endif