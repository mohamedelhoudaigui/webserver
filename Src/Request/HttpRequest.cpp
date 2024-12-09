#include "../../Include/HttpRequest.hpp"

HttpRequest::HttpRequest() : chunked(false), content_length(0) {}

HttpRequest::~HttpRequest() {}

const std::string& HttpRequest::getMethod() const {
    return method;
}

const std::string& HttpRequest::getUri() const {
    return uri;
}

const std::string& HttpRequest::getHttpVersion() const {
    return http_version;
}

const std::string& HttpRequest::getQuery() const {
    return query;
}

const std::string& HttpRequest::getPath() const {
    return path;
}

const std::string& HttpRequest::getBody() const {
    return body;
}

const std::map<std::string, std::string>& HttpRequest::getHeaders() const {
    return headers;
}

bool HttpRequest::isChunked() const {
    return chunked;
}

size_t HttpRequest::getContentLength() const {
    return content_length;
}