#include "../../Include/Storage.hpp"

ConfigFile::ConfigFile() {}

ConfigFile::ConfigFile(const ConfigFile &other) { *this = other; }


ConfigFile &ConfigFile::operator=(const ConfigFile &other) {
  if (this != &other) {
    this->servers = other.GetServers();
  }
  return *this;
}

ConfigFile::~ConfigFile() {}

//-------------------


const std::vector<ServerConf> &ConfigFile::GetServers() const {
  return (this->servers);
}

bool ConfigFile::CheckServer(const std::string &Host, unsigned int Port) const {
  for (size_t i = 0; i < this->servers.size(); ++i) {
    if (this->servers[i].GetHost() == Host &&
        this->servers[i].GetPort() == Port)
      return (true);
  }
  return (false);
}

const ServerConf &ConfigFile::GetServer(const std::string &Host,
                                        unsigned int Port) const {
  for (size_t i = 0; i < this->servers.size(); ++i) {
    if (this->servers[i].GetHost() == Host &&
        this->servers[i].GetPort() == Port)
      return (this->servers[i]);
  }
  throw std::runtime_error("No server match");
}

void  ConfigFile::SetEmptyServer()
{
  this->servers.push_back(ServerConf());
}

ServerConf&  ConfigFile::GetLastServer()
{
  return (this->servers.back());
}