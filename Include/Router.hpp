#pragma once

#include "Types.hpp"
#include "HttpRequest.hpp"
#include "HttpResponse.hpp"
#include "Config.hpp"
#include <string>

class Router {
private:
    Config* config;
    
    // Helper methods
    ServerConf& findMatchingServer(const std::string& host, unsigned int port);
    RouteConf& findMatchingLocation(ServerConf& server, const std::string& path);
    bool isMethodAllowed(const RouteConf& route, const std::string& method);
    std::string getFullPath(const RouteConf& route, const std::string& requestPath);

public:
    Router();
    Router(ConfigFile& config);
    ~Router();

    // Main routing method
    RouteConf& route(Request& request);
}; 