#include "../../Include/Storage.hpp"

ServerConf::ServerConf() {}

ServerConf::ServerConf(const std::string Host, const std::string ServerName,
                       const std::string Root, const unsigned int Port) {
  this->Host = Host;
  this->ServerName = ServerName;
  this->Root = Root;
  this->Port = Port;
}

ServerConf::ServerConf(const ServerConf &other) { *this = other; }

ServerConf &ServerConf::operator=(const ServerConf &other) {
  if (this != &other) {
    this->Host = other.GetHost();
    this->ServerName = other.GetServerName();
    this->Root = other.GetRoot();
    this->Port = other.GetPort();
    this->Routes = other.GetRoutes();
    this->ErrorPages = other.GetErrorPages();
  }
  return (*this);
}

ServerConf::~ServerConf() {}

//----------------------

const std::vector<RouteConf> &ServerConf::GetRoutes() const {
  return (this->Routes);
}

const std::map<unsigned int, std::string> &ServerConf::GetErrorPages() const {
  return (this->ErrorPages);
}

const std::string &ServerConf::GetHost() const { return (this->Host); }

const std::string &ServerConf::GetServerName() const {
  return (this->ServerName);
}

const std::string &ServerConf::GetRoot() const { return (this->Root); }

unsigned int ServerConf::GetPort() const { return (this->Port); }

bool ServerConf::CheckLocation(const std::string &LocationPath) const {
  for (size_t i = 0; i < this->Routes.size(); ++i) {
    if (LocationPath == this->Routes[i].GetLocationPath())
      return (true);
  }
  return (false);
}

const RouteConf &
ServerConf::GetLocation(const std::string &LocationPath) const {
  for (size_t i = 0; i < this->Routes.size(); ++i) {
    if (LocationPath == this->Routes[i].GetLocationPath())
      return (this->Routes[i]);
  }
  throw std::runtime_error("No matching location");
}

RouteConf& ServerConf::GetLastRoute() {
  return (this->Routes.back());
}

//----------------

void ServerConf::SetHost(const std::string &Host) { this->Host = Host; }

void ServerConf::SetServerName(const std::string &ServerName) {
  this->ServerName = ServerName;
}

void ServerConf::SetRoot(const std::string &Root) { this->Root = Root; }

void ServerConf::SetPort(unsigned int Port) { this->Port = Port; }

void ServerConf::SetErrorPage(const std::pair<unsigned int, std::string> &ErrorPage) {
  this->ErrorPages.insert(ErrorPage);
}

void  ServerConf::SetEmptyRoute() {
  this->Routes.push_back(RouteConf());
}