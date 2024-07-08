/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: demre <demre@student.42malaga.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/07 09:46:47 by blarger           #+#    #+#             */
/*   Updated: 2024/07/08 18:51:44 by demre            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Webserv.hpp"


int main(int argc, const char **argv)
{
	try
	{
    std::string configFile = argv[1] ? argv[1] : DEFAULT_CONFIG_PATH;
    
			// Webserv start(configFile);
		if (argc == 2)
			Webserv start(argv[1]);
		else
			Webserv start;
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