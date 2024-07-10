#pragma once

#ifndef POST_HPP
#define POST_HPP

#include "Webserv.hpp"

class POST : public Webserv
{

private:
  // Request line
  std::string pathToRessource;
  std::string HTTPversion;
  // Header
  std::string host;
  std::string userAgent;
  std::string accept;
  // Body
  std::string body;

public:
  POST(Webserv _server, int serverFD, int clientFD, std::string &clientInput);
  POST();
  ~POST(void);

};

#endif