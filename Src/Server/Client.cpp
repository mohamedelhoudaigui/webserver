#include "../../Include/Client.hpp"
#include "../../Include/HttpRequest.hpp"
#include <sstream>

Client::Client(Config* config)
    : fd(-1)
    , request_buffer()
    , response()
    , current_request(NULL)
    , requests_handled(0)
    , last_activity(time(NULL))
    , keep_alive(true)
    , config(config)
{
    // (void)config;
}

Client::~Client() {
    /*delete current_request;*/
}

Client::Client(const Client& other) {
    *this = other;
}

Client& Client::operator=(const Client& other) {
    if (this != &other) {
        fd = other.fd;
        request_buffer = other.request_buffer;
        response = other.response;
        current_request = other.current_request;
        requests_handled = other.requests_handled;
        last_activity = other.last_activity;
        keep_alive = other.keep_alive;
        config = other.config;
    }
    return *this;
}

void Client::updateLastActivity() {
    last_activity = time(NULL);
}

bool Client::hasTimedOut() const {
    return (time(NULL) - last_activity) > TIMEOUT;
}

bool Client::shouldClose() const {
    return !keep_alive || hasTimedOut() || requests_handled >= MAX_REQUESTS;
}

int Client::Recv(int BufferSize) {
	updateLastActivity();
    char Buffer[BufferSize];
    int count = recv(fd, Buffer, BufferSize, 0);
    if (count <= 0)
        return count;
    // Buffer[count - 1] = '\0';
    request_buffer.append(Buffer, count);

    try {
        processRequest();
        Send();
        return keep_alive ? 1 : -1;
    } catch (const std::exception& e) {
        response = "HTTP/1.1 400 Bad Request\r\n\r\n";
        return -1;
    }
}

void Client::processRequest() {
    std::istringstream stream(request_buffer);
    
    try {
        // Create appropriate request object based on HTTP method
        if (!current_request) {
            std::string first_line = request_buffer.substr(0, request_buffer.find("\r\n"));
            if (first_line.find("GET") == 0)
                current_request = new GET();
            else if (first_line.find("POST") == 0)
                current_request = new POST();
            else if (first_line.find("DELETE") == 0)
                current_request = new DELETE();
            else
                throw std::runtime_error("Invalid HTTP method");
        }

        // Parse the request
        current_request->parseRequest(request_buffer);

        // Get configuration and set up routing
        if (config) {
            ConfigFile config_file = config->GetResult();
            Router router(&config_file);
            
            try {
                // Route the request
                RouteConf& location = router.route(*current_request);
                
                // Handle the request based on configuration
                if (location.GetIsCgi()) {
                    handleCGI(location);
                } else {
                    handleStaticFile(location, route.findMatchingServer(ConfigFile::,));
                }
                
                // Update request statistics
                requests_handled++;
                keep_alive = current_request->isKeepAlive();
                
                // Clean up
                delete current_request;
                current_request = NULL;
                request_buffer.clear();
                
            } catch (const std::exception& e) {
                // Handle routing errors
                response = "HTTP/1.1 404 Not Found\r\n";
                response += "Content-Type: text/plain\r\n";
                response += "Content-Length: 21\r\n\r\n";
                response += "Resource not found\r\n";
                
                delete current_request;
                current_request = NULL;
                request_buffer.clear();
                return;
            }
        } else {
            // Configuration not available
            response = "HTTP/1.1 500 Internal Server Error\r\n";
            response += "Content-Type: text/plain\r\n";
            response += "Content-Length: 28\r\n\r\n";
            response += "Server configuration error\r\n";
            
            delete current_request;
            current_request = NULL;
            request_buffer.clear();
            return;
        }

    } catch (const std::exception& e) {
        // Clean up on error
        delete current_request;
        current_request = NULL;
        
        if (std::string(e.what()) == "Incomplete request")
            return;
            
        throw;
    }
}

void Client::buildResponse() {
    response = "HTTP/1.1 200 OK\r\n";
    response += "Content-Type: text/plain\r\n";
    response += "Content-Length: 0\r\n";
    
    if (keep_alive) {
        response += "Connection: keep-alive\r\n";
        response += "Keep-Alive: timeout=";
                //    tostring(TIMEOUT) + 
                //    ", max=" + 
                //    tostring(MAX_REQUESTS - requests_handled) + 
                //    "\r\n";
    } else {
        response += "Connection: close\r\n";
    }
    
    response += "\r\n";
}

void Client::Send() {
    send(fd, response.c_str(), response.size(), 0);
    response.clear();
}

void Client::SetFd(int fd) {
    this->fd = fd;
}

int Client::GetFd() const {
    return fd;
}

void Client::reset() {
    delete current_request;
    current_request = NULL;
    request_buffer.clear();
    response.clear();
    requests_handled = 0;
    last_activity = time(NULL);
    keep_alive = true;
}

void Client::handleCGI(RouteConf& location) {
	(void)location;
    // Basic CGI implementation
    // TODO: Implement full CGI handling
    response = "HTTP/1.1 501 Not Implemented\r\n";
    response += "Content-Type: text/plain\r\n";
    response += "Content-Length: 21\r\n";
    response += "\r\n";
    response += "CGI not implemented yet";
}

void Client::handleStaticFile(RouteConf& location, ServerConf& server) {
    std::string path = current_request->getPath();
    std::string full_path = server.GetRoot();
    
    // If path is root, use index
    if (path == "/" || path.empty()) {
        full_path += "/" + location.GetIndex();
    } else {
        full_path += path;
    }
    
    std::ifstream file(full_path.c_str(), std::ios::binary | std::ios::ate);
    
    if (!file.is_open()) {
        response = "HTTP/1.1 404 Not Found\r\n";
        response += "Content-Type: text/plain\r\n";
        response += "Content-Length: 21\r\n\r\n";
        response += "Resource not found\r\n";
        return;
    }

    std::streamsize size = file.tellg();
    file.seekg(0, std::ios::beg);

    std::vector<char> buffer(size);
    if (file.read(buffer.data(), size)) {
        std::stringstream ss;
        ss << size;
        
        response = "HTTP/1.1 200 OK\r\n";
        response += "Content-Type: text/html\r\n";
        response += "Content-Length: " + ss.str() + "\r\n";
        response += "Connection: close\r\n\r\n";
        response.append(buffer.data(), size);
    } else {
        response = "HTTP/1.1 500 Internal Server Error\r\n";
        response += "Content-Type: text/plain\r\n";
        response += "Content-Length: 22\r\n\r\n";
        response += "Error reading file\r\n";
    }
}