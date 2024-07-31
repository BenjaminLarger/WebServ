/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: demre <demre@student.42malaga.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/07 09:46:47 by blarger           #+#    #+#             */
/*   Updated: 2024/07/31 14:15:08 by demre            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Webserv.hpp"

int main(int argc, const char **argv)
{
  try
  {
    if (argc > 2)
      throw(std::runtime_error(
          "Too many arguments. Use: ./webserv [configuration file]"));

    std::string configFile = argv[1] ? argv[1] : DEFAULT_CONFIG_PATH;
    ServerConfig start(configFile.c_str());
  }
  catch (const std::exception &e)
  {
    std::cerr << RED << "ERROR: " << RESET << e.what() << std::endl;
    exit(EXIT_FAILURE);
  }
  catch (...)
  {
    std::cerr << RED << "Unknown error!" << RESET << std::endl;
    exit(EXIT_FAILURE);
  }

  return (0); // eventually replace with exit(EXIT_SUCCESS);
}
