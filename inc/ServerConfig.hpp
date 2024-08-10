/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerConfig.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: demre <demre@student.42malaga.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/31 18:12:40 by demre             #+#    #+#             */
/*   Updated: 2024/08/10 15:07:05 by demre            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "dependencies.hpp"

// ************************************************************************** //
//                               Class //
// ************************************************************************** //

struct LocationConfig
{
  // Define a list of accepted HTTP methods for the route
  std::vector<std::string> allowedMethods; // allow_methods GET POST DELETE

  // Define an HTTP redirection
  std::pair<int, std::string>
      redirection; // return 301 http://example.org$new_uri;

  // Define a directory or a file from where the file should be searched
  std::string root;       // root /var/www;
  std::string alias;      // alias /temp/www;
  std::string serverPath; // resolve path on server: ./temp/www/URI

  // Turn on or off directory listing
  bool autoIndexOn; // autoindex on/off;

  // Set a default file to answer if the request is a directory
  std::string index; // index index.html;
};

class ServerConfig
{
private:
public:
  ServerConfig(void);
  ~ServerConfig();

  std::string host;                     // 127.0.0.1
  int port;                             // listen 8080;
  long long maxBodySize;                // client_max_body_size 1000;
  std::vector<std::string> serverNames; // server_names ex.com www.ex.com;
  std::string serverRoot;               // root /var/www
  std::string serverIndex;              // index index.html
  std::string serverPath;               // resolve path on server: ./var/www/URI
  std::map<int, std::string> errorPages;           // error_page 404 /404.html;
  std::map<std::string, LocationConfig> locations; // urlPattern, location block

  // Returns a list of all the server names as one string (to display in terminal)
  std::string getServerNames(void) const;

  // Returns the server name at the given index in the serverNames vector
  const std::string &getServerName(int index) const;

  const int &getPort(void) const;
  const std::string &getHost(void) const;

  // Add a server name to the serverNames vector
  void addServerName(std::string newServerName);
  // Set the port
  void addPort(int newPort);

  // Resets the attributes from the current ServerConfig object
  void reset(void);

  // Parses server configs to a vector of ServerConfig from a config file
  static std::vector<ServerConfig> parseConfigs(const char *filename);

  // Check line has correct end character and trim ws if required
  static void validateAndSanitizeServerLine(std::string &line,
                                            std::stringstream &ss,
                                            std::string &key,
                                            std::ifstream &file);

  // Check line has correct end character and trim ws if required
  static void validateAndSanitizeLocationLine(std::string &line,
                                              std::stringstream &ss,
                                              std::string &key,
                                              std::ifstream &file);

  // Handles ServerConfig when reaching the end of a server block in the configfile
  void endServerBlock(bool &insideServerBlock,
                      std::vector<ServerConfig> &serverConfigs,
                      std::vector<int> &tempPorts, std::ifstream &file);

  // Checks that the ServerConfig has valid and complete data
  bool checkConfig(std::vector<int> &tempPorts);

  void parseLocationBlock(std::ifstream &file, std::string urlPattern);
  void resolveServerPathForLocations(void);
};
