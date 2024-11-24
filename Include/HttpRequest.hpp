#pragma once

#include <string>
#include <iostream>
#include <sstream>
#include "HttpHeaders.hpp"

class HttpRequest {
    private:
        std::string method;
        std::string uri;
        std::string http_version;
        static const char*  valid_methods[];
        bool    isValidMethod(const std::string& method) const;
        bool    isValidUri(const std::string& uri) const;
    public:
        HttpRequest();
        bool Parser(const std::string& line);
        const   std::string& getMethod() const;
        const   std::string& getUri() const;
        void    parseUri(std::string &path, std::string &query) const;
        ~HttpRequest();
};


//required http request line 
// Method SP Request-URI SP HTTP-Version CRLF
//required headers at least host is mandatory

/*
    GET /index.html HTTP/1.1\r\n
    Host: localhost:8080\r\n
    Connection: keep-alive\r\n
    Accept: text/html\r\n
    \r\n
*/