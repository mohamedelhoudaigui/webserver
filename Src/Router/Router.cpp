#include "../../Include/Router.hpp"
#include <stdexcept>

Router::Router(ConfigFile *config) { this->config = config; }

Router::~Router() {}

ServerConf &Router::findMatchingServer(std::string &host, unsigned int port) {
  if (config->CheckServer(host, port)) {
    return (config->GetServer(host, port));
  }
  // return default server
  throw std::runtime_error("No matching server found");
}

RouteConf &Router::findMatchingLocation(ServerConf &server, std::string &path) {
  if (!server.CheckLocation(path))
    throw std::runtime_error("No matching location found");
  return (server.GetLocation(path));
}

bool Router::isMethodAllowed(RouteConf &route, std::string &method) {
  return (route.CheckMethod(method));
}

std::string Router::getFullPath(RouteConf &route, std::string &path) {
  std::string server_root = route.GetRoot();
  server_root += path;
  if (server_root[server_root.size() - 1] == '/' && route.CheckAutoIndex())
    server_root += route.GetIndex();
  return (server_root);
}

RouteConf &Router::route(Request &request) {
  // Extract host and port from request headers
  std::string host = request.getHeaders().at("Host");
  size_t colonPos = host.find(':');
  std::string hostname = host.substr(0, colonPos);
  unsigned int port = colonPos != std::string::npos
                          ? std::atoi(host.substr(colonPos + 1).c_str())
                          : -1;

  // Find matching server
  ServerConf &server = findMatchingServer(hostname, port);

  // Find matching location
  RouteConf &location = findMatchingLocation(server, const_cast<std::string&>(request.getPath()));

  // Check if method is allowed
  if (!isMethodAllowed(location, const_cast<std::string&>(request.getMethod()))) {
    throw std::runtime_error("Method not allowed");
  }

  // Handle redirections
  if (location.CheckRedirection()) {
    throw std::runtime_error("Redirect:" + location.GetRedirection());
  }

  return location;
}
