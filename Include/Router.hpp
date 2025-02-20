#pragma once

#include "HttpRequest.hpp"
#include "Types.hpp"
#include "Storage.hpp"

// Forward declaration
class Request;

class Router {
	private:
		ConfigFile *config;

		// Helper methods
		RouteConf &findMatchingLocation(ServerConf &server, std::string &path);
		bool isMethodAllowed(RouteConf &route, std::string &method);
		std::string getFullPath(RouteConf &route, std::string &requestPath);

	public:
		Router();
		Router(ConfigFile* config);
		~Router();
		ServerConf &findMatchingServer(std::string &host, unsigned int port);

		// Main routing method
		RouteConf& route(Request &request);
};
