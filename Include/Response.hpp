#pragma once

#include <iostream>
#include <map>

class HttpResponse {
//Attributes
    private:
        std::map<std::string, std::string>  _headers;
        std::string _version;
        std::string _reason;
        std::string _body;
        int         _statusCode;
    public:
//Constructors
        HttpResponse();
        HttpResponse(std::map<std::string, std::string> headers,\
         std::string version,\
         std::string reason,\
         std::string body,
         int statusCode);
        HttpResponse(const HttpResponse& obj);
//Operators Overload
        HttpResponse& operator=(const HttpResponse& obj);
        friend std::ostream& operator<<(std::ostream& out, const HttpResponse& obj);
//Accessors
        void    setVersion(std::string& version);
        void    setReason(std::string& version);
        void    setBody(std::string& version);
        void    setStatusCode(int status);
        void    setHeaders(std::map<std::string, std::string> headers);
        std::map<std::string, std::string> getHeaders() const;
        std::string getVersion() const;
        std::string getReason() const;
        std::string getBody() const;
        int      getStatusCode() const;
};
