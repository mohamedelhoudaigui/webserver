#include "../../Include/Router.hpp"

Router::Router(ConfigFile *config) { this->config = config; }

Router::~Router() {}

ServerConf &Router::findMatchingServer(std::string &host, unsigned int port) {
  if (config->CheckServer(host, port)) {
    return (config->GetServer(host, port));
  }
  // return default server here
}

RouteConf &Router::findMatchingLocation(ServerConf &server, std::string &path) {
  std::string bestMatch;
  size_t bestLength = 0;
  RouteConf *matchedRoute = NULL;

  // Find the longest matching location prefix
  for (std::vector<RouteConf>::iterator it = server.Routes.begin();
       it != server.Routes.end(); ++it) {
    std::string locationPath = it->Location;
    // Check for exact match
    if (locationPath == path) {
      return *it;
    }
    // Check for prefix match
    if (path.substr(0, locationPath.length()) == locationPath) {
      if (locationPath.length() > bestLength) {
        bestLength = locationPath.length();
        matchedRoute = &(*it);
      }
    }
  }

  if (matchedRoute) {
    return *matchedRoute;
  }

  throw std::runtime_error("No matching location found");
}

bool Router::isMethodAllowed(RouteConf &route, std::string &method) {
  return route.CheckMethod(method);
}

std::string Router::getFullPath(const RouteConf &route,
                                const std::string &requestPath) {
  std::string path = route.GetRoot();

  // Remove location prefix from request path
  std::string relativePath = requestPath.substr(route.Location.length());

  // Combine with root path
  if (!relativePath.empty() && relativePath[0] != '/') {
    path += "/";
  }
  path += relativePath;

  // Handle index file
  if (path[path.length() - 1] == '/') {
    path += route.GetIndex();
  }

  return path;
}

RouteConf &Router::route(Request &request) {
  // Extract host and port from request headers
  std::string host = request.getHeaders().at("Host");
  size_t colonPos = host.find(':');
  std::string hostname = host.substr(0, colonPos);
  unsigned int port = colonPos != std::string::npos
                          ? std::atoi(host.substr(colonPos + 1).c_str())
                          : 80;

  // Find matching server
  ServerConf &server = findMatchingServer(hostname, port);

  // Find matching location
  RouteConf &location = findMatchingLocation(server, request.getPath());

  // Check if method is allowed
  if (!isMethodAllowed(location, request.getMethod())) {
    throw std::runtime_error("Method not allowed");
  }

  // Handle redirections
  if (location.CheckRedirection()) {
    throw std::runtime_error("Redirect:" + location.GetRedirection());
  }

  return location;
}
