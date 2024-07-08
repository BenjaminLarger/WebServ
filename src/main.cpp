/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blarger <blarger@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/07 09:46:47 by blarger           #+#    #+#             */
/*   Updated: 2024/07/08 18:27:08 by blarger          ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#include "Webserv.hpp"

int main(int argc, const char **argv)
{
	try
	{
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