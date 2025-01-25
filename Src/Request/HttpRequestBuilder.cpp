#include "../../Include/HttpRequest.hpp"


bool    HttpRequestBuilder::validateMethod(const std::string& method) const {
    return method == "GET" || method == "POST" || method == "DELETE";
}

bool    HttpRequestBuilder::validateUri(const std::string& uri) const {
    if (uri.empty() || uri[0] != '/')
        return false;
    for (size_t i = 0; i < uri.length(); i++) {
        if (!isprint(uri[i]) || uri[i] == ' ')
            return false;
    }
    return true;
}

bool    HttpRequestBuilder::validateVersion(const std::string& version) const {
    return version == "HTTP/1.1";
}

bool    HttpRequestBuilder::validateContentLength(const std::string& length) const {
    for (std::string::const_iterator it = length.begin(); it != length.end(); it++) {
        if (!isdigit(*it))
            return false;
    }
    return true;
}

HttpRequestBuilder::HttpRequestBuilder() : request(new HttpRequest()) {}
HttpRequestBuilder::~HttpRequestBuilder() {
    // delete request;
}

HttpRequestBuilder& HttpRequestBuilder::setMethod(const std::string& method) {
    if (validateMethod(method))
        request->method = method;
    return *this;
}

HttpRequestBuilder& HttpRequestBuilder::setUri(const std::string& uri) {
    if (validateUri(uri)) {
        request->uri = uri;
        parseUri();
    }
    return *this;
}

HttpRequestBuilder& HttpRequestBuilder::setVersion(const std::string& version) {
    if (validateVersion(version))
        request->http_version = version;
    return *this;
}

void HttpRequestBuilder::parseUri() {
    std::string::size_type query_pos = request->uri.find('?');
    if (query_pos != std::string::npos) {
        request->path = request->uri.substr(0, query_pos);
        request->query = request->uri.substr(query_pos + 1);
    } else {
        request->path = request->uri;
        request->query = "";
    }
}

HttpRequestBuilder& HttpRequestBuilder::addHeader(const std::string& key, const std::string& value) {
    if (HttpHeaders::isValidHeader(key + ": " + value))
        request->headers[key] = value;
    return *this;
}

HttpRequestBuilder& HttpRequestBuilder::setBody(const std::string& body) {
    request->body = body;
    return *this;
}

bool HttpRequestBuilder::parseRequestLine(const std::string& line) {
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

bool HttpRequestBuilder::parseHeaders(std::istringstream& stream) {
    std::string line;
    bool host_found = false;

    while (std::getline(stream, line) && !line.empty() && line != "\r") {
        if (line[line.length() - 1] == '\r')
            line.erase(line.length() - 1);
        
        std::string::size_type colon = line.find(':');
        if (colon == std::string::npos)
            return false;

        std::string key = line.substr(0, colon);
        std::string value = line.substr(colon + 1);
        HttpHeaders::trim(value, " \t");

        if (key == "Host") {
            if (!HttpHeaders::isValidHost(value))
                return false;
            host_found = true;
        }
        else if (key == "Content-Length") {
            if (!validateContentLength(value))
                return false;
            request->content_length = std::atol(value.c_str());
        }
        else if (key == "Transfer-Encoding" && value == "chunked") {
            request->chunked = true;
        }
        
        addHeader(key, value);
    }

    return host_found;
}

bool HttpRequestBuilder::parseRequest(const std::string& raw_request)
{
    std::istringstream stream(raw_request);
    std::string request_line;
    
    if (!std::getline(stream, request_line)) return false;
    if (request_line[request_line.length() - 1] == '\r') request_line.erase(request_line.length() - 1);
    if (!parseRequestLine(request_line))
        return false;
    if (!parseHeaders(stream))
        return false;
    if (request->chunked)
        return parseChunkedBody(stream);
    else if (request->content_length > 0)
        return parseBody(stream);

    return true;
}

HttpRequest* HttpRequestBuilder::build()
{
    HttpRequest* built_request = request;
    request = new HttpRequest();
    return built_request;
}

void HttpRequestBuilder::reset() {
    delete request;
    request = new HttpRequest();
}

// HttpRequestParser implementation
HttpRequestParser::HttpRequestParser(size_t max_size)
    : state(REQUEST_LINE), max_request_size(max_size) {}

bool HttpRequestParser::feed(const std::string& data)
{
    if (buffer.length() + data.length() > max_request_size) {
        state = ERROR;
        return false;
    }
    buffer += data;
    while (true) {
        switch (state) {
            case REQUEST_LINE:
                if (!parseRequestLine())
                    return false;
                break;
            case HEADERS:
                if (!parseHeaders())
                    return false;
                break;
            case BODY:
                if (builder.getRequest()->isChunked())
                    return parseChunkedBody();
                else
                    return parseBody();
                break;
            case COMPLETE:
                return true;
            case ERROR:
                return false;
        }
    }
}

bool HttpRequestParser::isComplete() const {
    return state == COMPLETE;
}

HttpRequest* HttpRequestParser::getRequest() {
    if (state == COMPLETE)
        return builder.build();
    return NULL;
}

void HttpRequestParser::reset() {
    builder.reset();
    buffer.clear();
    state = REQUEST_LINE;
}