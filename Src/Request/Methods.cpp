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
	(void)stream;
    // GET requests shouldn't have a body according to HTTP/1.1
    if (content_length > 0 || chunked)
        throw std::runtime_error("GET request should not have a body");
    body.clear();
}

// POST Implementation
POST::POST() : Request(){}

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
    // Require Content-Length or chunked for POST
    if (!chunked && headers.find("Content-Length") == headers.end())
        throw std::runtime_error("POST request requires Content-Length or chunked encoding");
}

void POST::parseBody(std::istringstream& stream) {
    if (chunked)
        Request::parseChunkedBody(stream);
    else
        Request::parseBody(stream);
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

bool DELETE::validateUri(const std::string& uri) const {
    if (uri.empty() || uri[0] != '/') {
        return false;
    }
	return true;
}

void DELETE::parseBody(std::istringstream& stream) {
    // DELETE requests may have a body, but it's uncommon
    if (content_length > 0 || chunked)
        Request::parseBody(stream);
}
