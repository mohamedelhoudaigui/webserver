#include "../../Include/Client.hpp"
#include "../../Include/HttpRequest.hpp"

Client::Client()
    : fd(-1)
    , requests_handled(0)
    , last_activity(time(NULL))
    , keep_alive(true)
{
    current_request = NULL;
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

        if (!current_request) // Additional safety check
            throw std::runtime_error("Failed to create request object");

        current_request->parseRequest(request_buffer);
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
