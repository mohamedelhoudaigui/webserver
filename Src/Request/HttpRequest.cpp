#include "../../Include/HttpRequest.hpp"

const char* valid_methods[] = {"GET", "POST", "DELETE"};

HttpRequest::HttpRequest(std::string& RawRequest)
{

}

HttpRequest::~HttpRequest() {}

bool HttpRequest::isValidVersion(const std::string& version) const 
{
    return version == "HTTP/1.1";
}

bool    HttpRequest::isValidMethod(const std::string& method) const
{
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

bool    HttpRequest::Parser(const std::string& line)
{
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

    //HTTP Version
    start = end + 1;
    end = line.find("\r\n", start);
    if (end == std::string::npos)
        return false;
    http_version = line.substr(start, end - start);
    if (!isValidVersion(http_version))
        return false;

    return true;
}


void    HttpRequest::parseUri(std::string &path, std::string &query) const
{
    std::string::size_type query_pos = uri.find('?');
    if (query_pos != std::string::npos)
    {
        path = uri.substr(0, query_pos);
        query = uri.substr(query_pos + 1);
    }
    else
    {
        path = uri;
        query = "";
    }
}

const std::string& HttpRequest::getMethod() const { return method; }
const std::string& HttpRequest::getUri() const { return uri; }
const std::string& HttpRequest::getHttpVersion() const { return http_version; }