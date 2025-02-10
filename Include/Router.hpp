#pragma once

#include "HttpRequest.hpp"
#include "HttpResponse.hpp"
#include "Types.hpp"

class Router {
private:
  ConfigFile *config;

  // Helper methods
  ServerConf &findMatchingServer(std::string &host, unsigned int port);
  RouteConf &findMatchingLocation(ServerConf &server, std::string &path);
  bool isMethodAllowed(RouteConf &route, std::string &method);
  std::string getFullPath(RouteConf &route,
                          std::string &requestPath);

public:
  Router();
  Router(ConfigFile* config);
  ~Router();

  // Main routing method
  RouteConf &route(Request &request);
};
