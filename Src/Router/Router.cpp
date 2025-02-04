#include "../../Include/Router.hpp"

Router::Router(ConfigFile& config) : config(config) {}

Router::~Router() {}

ServerConf& Router::findMatchingServer(const std::string& host, unsigned int port) {
    try {
        return config.GetServer(const_cast<std::string&>(host), port);
    } catch (const std::runtime_error&) {
        // If no exact match, try to find server with matching port only
        for (std::vector<ServerConf>::iterator it = config.servers.begin(); 
             it != config.servers.end(); ++it) {
            if (it->Port == port) {
                return *it;
            }
        }
        throw std::runtime_error("No matching server found");
    }
}

RouteConf& Router::findMatchingLocation(ServerConf& server, const std::string& path) {
    std::string bestMatch;
    size_t bestLength = 0;
    RouteConf* matchedRoute = NULL;

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

bool Router::isMethodAllowed(const RouteConf& route, const std::string& method) {
    return route.CheckMethod(const_cast<std::string&>(method));
}

std::string Router::getFullPath(const RouteConf& route, const std::string& requestPath) {
    std::string path = route.GetRoot();
    
    // Remove location prefix from request path
    std::string relativePath = requestPath.substr(route.Location.length());
    
    // Combine with root path
    if (!relativePath.empty() && relativePath[0] != '/') {
        path += "/";
    }
    path += relativePath;
    
    // Handle index file
    if (path[path.length()-1] == '/') {
        path += route.GetIndex();
    }
    
    return path;
}

RouteConf& Router::route(Request& request) {
    // Extract host and port from request headers
    std::string host = request.getHeaders().at("Host");
    size_t colonPos = host.find(':');
    std::string hostname = host.substr(0, colonPos);
    unsigned int port = colonPos != std::string::npos ? 
        std::atoi(host.substr(colonPos + 1).c_str()) : 80;

    // Find matching server
    ServerConf& server = findMatchingServer(hostname, port);

    // Find matching location
    RouteConf& location = findMatchingLocation(server, request.getPath());

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