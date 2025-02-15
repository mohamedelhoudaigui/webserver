#include "../../Include/Client.hpp"
#include "../../Include/HttpRequest.hpp"

Client::Client(Config* config)
    : fd(-1)
    , requests_handled(0)
    , last_activity(time(NULL))
    , keep_alive(true)
    , config(config)
{
    current_request = NULL;
}

Client::~Client() {
    /*delete current_request;*/
}

Client::Client(const Client& other) 
    : fd(-1)
    , requests_handled(0)
    , last_activity(time(NULL))
    , keep_alive(true)
    , config(other.config)
    , current_request(NULL)
{
    if (other.current_request) {
        // Deep copy based on type
        if (dynamic_cast<GET*>(other.current_request))
            current_request = new GET(*dynamic_cast<GET*>(other.current_request));
        else if (dynamic_cast<POST*>(other.current_request))
            current_request = new POST(*dynamic_cast<POST*>(other.current_request));
        else if (dynamic_cast<DELETE*>(other.current_request))
            current_request = new DELETE(*dynamic_cast<DELETE*>(other.current_request));
    }
    request_buffer = other.request_buffer;
    response = other.response;
}

Client& Client::operator=(const Client& other) {
    if (this != &other) {
        delete current_request;
        current_request = NULL;
        
        if (other.current_request) {
            // Deep copy based on type
            if (dynamic_cast<GET*>(other.current_request))
                current_request = new GET(*dynamic_cast<GET*>(other.current_request));
            else if (dynamic_cast<POST*>(other.current_request))
                current_request = new POST(*dynamic_cast<POST*>(other.current_request));
            else if (dynamic_cast<DELETE*>(other.current_request))
                current_request = new DELETE(*dynamic_cast<DELETE*>(other.current_request));
        }
        
        fd = other.fd;
        request_buffer = other.request_buffer;
        response = other.response;
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


        current_request->parseRequest(request_buffer);
        ConfigFile config_file = config->GetResult();
        Router router(&config_file);
        RouteConf& location = router.route(*current_request);
        if (location.CheckIsCgi())
            handleCGI(location);
        else
            handleStaticFile(location);
        buildResponse();
        requests_handled++;
        keep_alive = current_request->isKeepAlive();
        delete current_request;
        current_request = NULL; // Use helper method instead of direct delete
        request_buffer.clear();
    } catch (const std::exception& e) {
        delete current_request;
        current_request = NULL; // Ensure cleanup on error
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
    // Basic CGI implementation
    // TODO: Implement full CGI handling
    response = "HTTP/1.1 501 Not Implemented\r\n";
    response += "Content-Type: text/plain\r\n";
    response += "Content-Length: 21\r\n";
    response += "\r\n";
    response += "CGI not implemented yet";
}

void Client::handleStaticFile(RouteConf& location) {
    // Basic static file implementation
    // TODO: Implement full static file handling
    response = "HTTP/1.1 501 Not Implemented\r\n";
    response += "Content-Type: text/plain\r\n";
    response += "Content-Length: 28\r\n";
    response += "\r\n";
    response += "Static files not implemented yet";
}
