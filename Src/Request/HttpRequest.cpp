#include "../../Include/HttpRequest.hpp"

Request::Request() : chunked(false), content_length(0) {}

Request::~Request() {}

const std::string& Request::getMethod() const {
    return method;
}

const std::string& Request::getUri() const {
    return uri;
}

const std::string& Request::getHttpVersion() const {
    return http_version;
}

const std::string& Request::getQuery() const {
    return query;
}

const std::string& Request::getPath() const {
    return path;
}

const std::string& Request::getBody() const {
    return body;
}

const std::map<std::string, std::string>& Request::getHeaders() const {
    return headers;
}

bool Request::isChunked() const {
    return chunked;
}

size_t Request::getContentLength() const {
    return content_length;
}

void Request::parseRequestLine(const std::string& line) {
    std::string::size_type first_space = line.find(' ');
    std::string::size_type last_space = line.rfind(' ');
    
    if (first_space == std::string::npos || last_space == std::string::npos || first_space == last_space)
        return false;

    std::string method = line.substr(0, first_space);
    std::string uri = line.substr(first_space + 1, last_space - first_space - 1);
    std::string version = line.substr(last_space + 1);

    return setMethod(method)
           .setUri(uri)
           .setVersion(version)
           .request != NULL;
}

int Request::parseHeaders(std::istringstream& stream) {
    std::string line;
    bool host_found = false;

    while (std::getline(stream, line) && !line.empty() && line != "\r") {
        if (line[line.length() - 1] == '\r')
            line.erase(line.length() - 1);
        
        std::string::size_type colon = line.find(':');
        if (colon == std::string::npos)
            return host_found;

        std::string key = line.substr(0, colon);
        std::string value = line.substr(colon + 1);
        HttpHeaders::trim(value, " \t");

        if (key == "Host") {
            if (!HttpHeaders::isValidHost(value))
                return host_found;
            host_found = true;
        }
        else if (key == "Content-Length") {
            if (!validateContentLength(value))
                return false;
            this->content_length = std::atol(value.c_str());
        }
        else if (key == "Transfer-Encoding" && value == "chunked") {
            this->isChunked = true;
        } else break;
        
        addHeader(key, value);
    }

    return host_found;
}