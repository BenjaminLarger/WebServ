/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: demre <demre@student.42malaga.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/07 09:46:47 by blarger           #+#    #+#             */
/*   Updated: 2024/08/02 13:39:59 by demre            ###   ########.fr       */
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

    // Use provided config file or default file
    std::string configFile = argv[1] ? argv[1] : DEFAULT_CONFIG_PATH;

    // Save server configurations to a vector of ServerConfig
    std::vector<ServerConfig> serverConfigs
        = ServerConfig::parseConfig(configFile.c_str());

    // Display server configs in terminal to check
    std::cout << "serverConfigs.size(): " << serverConfigs.size() << std::endl;
    for (size_t i = 0; i < serverConfigs.size(); i++)
    {
      std::cout << "ServerConfig " << i
                << ", host: " << serverConfigs[i].getHost()
                << ", port: " << serverConfigs[i].getPort()
                << ", server_names: " << serverConfigs[i].getServerNames()
                << std::endl;
    }

    // Launch servers passing serverConfigs vector
    if (serverConfigs.size() > 0)
      Webserv servers(serverConfigs);
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
