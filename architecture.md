// Loads and processes the config file
class ConfigServerFile
  .parsConfigFile()
    std::vector< ConfigSingleServer > serverConfigs // config of each server { }
  
  .check() // Research what nginx does when wrong data
    - different port for each server
    - valid elements in server { }


  .launchServers()
    // fork for  ?
    for (size_t i = 0; i < serverConfigs.size(); i++)
    {
      Webserv(serverConfigs[i])
    }

  .cleanup() // ?

  .parseString()
  .parseNum()

// Holds all config info for one server
class ConfigSingleServer 

// One web server running
class Webserv
  .bind()
  .listen()
  while (true)
    acceptNewConnection() -> Connection

// One client connected to the server?
class Connection