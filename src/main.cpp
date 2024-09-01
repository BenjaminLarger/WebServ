/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: demre <demre@student.42malaga.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/07 09:46:47 by blarger           #+#    #+#             */
/*   Updated: 2024/09/01 14:00:21 by demre            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Webserv.hpp"
#include "core.hpp"
#include "utils.hpp"

int main(int argc, const char **argv)
{
  try
  {
    if (argc > 2)
      throw(std::runtime_error(
          "Too many arguments. Use: ./webserv [configuration file]"));

    // Use provided config file or default file
    std::string configFile = argv[1] ? argv[1] : DEFAULT_CONFIG_PATH;

    // Save server configurations to a vector of ServerConfig
    std::vector<ServerConfig> serverConfigs
        = ServerConfig::parseConfigs(configFile.c_str());

    // Display server configs in terminal to check
    displayServerConfigs(serverConfigs);

    // Launch servers passing serverConfigs vector
    if (serverConfigs.size() > 0)
      Webserv servers(serverConfigs);
  }
  catch (const HttpException &e)
  {
    std::cerr << RED << "HttpException: " << RESET << e.what() << std::endl;
    exit(EXIT_FAILURE);
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

  exit(EXIT_SUCCESS);
}
