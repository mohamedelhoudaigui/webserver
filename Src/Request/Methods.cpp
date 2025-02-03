#include "../../Include/HttpRequest.hpp"

// GET Implementation
GET::GET() : Request() {}

GET::~GET() {}

void GET::parseRequestLine(const std::string& line) {
    Request::parseRequestLine(line);
    if (method != "GET")
        throw std::runtime_error("Invalid method for GET request");
}

bool GET::validateMethod(const std::string& method) const {
    return method == "GET";
}

void GET::parseBody(std::istringstream& stream) {
    // GET requests shouldn't have a body according to HTTP/1.1
    if (content_length > 0 || chunked)
        throw std::runtime_error("GET request should not have a body");
    body.clear();
}

// POST Implementation
POST::POST() : Request(), multipart(false), boundary("") {}

POST::~POST() {}

void POST::parseRequestLine(const std::string& line) {
    Request::parseRequestLine(line);
    if (method != "POST")
        throw std::runtime_error("Invalid method for POST request");
}

bool POST::validateMethod(const std::string& method) const {
    return method == "POST";
}

void POST::parseHeaders(std::istringstream& stream) {
    Request::parseHeaders(stream);
    
    // Check Content-Type for multipart
    std::map<std::string, std::string>::const_iterator it = headers.find("Content-Type");
    if (it != headers.end()) {
        if (it->second.find("multipart/form-data") != std::string::npos) {
            multipart = true;
            size_t pos = it->second.find("boundary=");
            if (pos != std::string::npos)
                boundary = it->second.substr(pos + 9);
            else
                throw std::runtime_error("Multipart form-data missing boundary");
        }
    }
    
    // Require Content-Length or chunked for POST
    if (!chunked && headers.find("Content-Length") == headers.end())
        throw std::runtime_error("POST request requires Content-Length or chunked encoding");
}

void POST::parseBody(std::istringstream& stream) {
    if (multipart)
        parseMultipartBody(stream);
    else
        Request::parseBody(stream);
}

void POST::parseMultipartBody(std::istringstream& stream) {
    std::string line;
    std::string current_part;
    bool in_part = false;
    
    while (std::getline(stream, line)) {
        if (line[line.length() - 1] == '\r')
            line.erase(line.length() - 1);
            
        if (line == "--" + boundary) {
            if (in_part) {
                parts.push_back(current_part);
                current_part.clear();
            }
            in_part = true;
        }
        else if (line == "--" + boundary + "--") {
            if (in_part)
                parts.push_back(current_part);
            break;
        }
        else if (in_part) {
            current_part += line + "\r\n";
        }
    }
}

const std::vector<std::string>& POST::getParts() const {
    return parts;
}

// DELETE Implementation
DELETE::DELETE() : Request() {}

DELETE::~DELETE() {}

void DELETE::parseRequestLine(const std::string& line) {
    Request::parseRequestLine(line);
    if (method != "DELETE")
        throw std::runtime_error("Invalid method for DELETE request");
}

bool DELETE::validateMethod(const std::string& method) const {
    return method == "DELETE";
}

void DELETE::validateUri() const {
    if (path.empty() || path == "/")
        throw std::runtime_error("DELETE request must specify a resource");
}

void DELETE::parseBody(std::istringstream& stream) {
    // DELETE requests may have a body, but it's uncommon
    if (content_length > 0 || chunked)
        Request::parseBody(stream);
} 