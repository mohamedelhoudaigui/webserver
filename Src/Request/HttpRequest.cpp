#include "../../Include/HttpRequest.hpp"

HttpRequest::HttpRequest() {}
HttpRequest::~HttpRequest() {}

bool    HttpRequest::isValidMethod(const std::string& method) const {
    for (short i = 0; i < 3; i++) {
        if (method == valid_methods[i])
            return true;
    }
    return false;
}

bool    HttpRequest::isValidUri(const std::string& uri) const {
    //must start with /
    if (uri.empty() || uri[0] != '/')
        return false;
    //valid chars only
    for (size_t i = 0; i < uri.length(); i++) {
        if (!isprint(uri[i]) || uri[i] == ' ')
            return false;
    }
    return true;
}

bool    HttpRequest::Parser(const std::string& line) {
    std::string::size_type start = 0;
    std::string::size_type end = 0;
    end = line.find(' ', start);
    //method
    if (end == std::string::npos)
        return false;
    method = line.substr(start, end);
    if (!isValidMethod(method))
        return false;
    //uri
    start = end + 1;
    end = line.find(' ', start);
    if (end == std::string::npos)
        return false;
    uri = line.substr(start, end - start);
    if (!isValidUri(uri))
        return false;
    return true;
}

const std::string& HttpRequest::getMethod() const { return method; }
const std::string& HttpRequest::getUri() const { return uri; }

void    HttpRequest::parseUri(std::string &path, std::string &query) const {}
