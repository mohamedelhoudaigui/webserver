#include "Response.hpp"

HttpResponse::HttpResponse() : _version("") , _reason(""), \
_body(""),  _statusCode(0){

};
HttpResponse::HttpResponse(std::map<std::string, std::string> headers,\
 std::string version,\
 std::string reason,\
 std::string body,
 int statusCode) {
    _headers = headers;
    _version = version;
    _reason = reason;
    _body = body;
    _statusCode = statusCode;
 };

HttpResponse::HttpResponse(const HttpResponse& obj) {
    _headers = obj._headers;
    _version = obj._version;
    _reason = obj._reason;
    _body = obj._body;
    _statusCode = obj._statusCode;
}

HttpResponse& HttpResponse::operator=(const HttpResponse& obj) {
    if (this != &obj) {
        _headers = obj._headers;
        _version = obj._version;
        _reason = obj._reason;
        _body = obj._body;
        _statusCode = obj._statusCode;
    }
    return *this;
}

std::ostream& operator<<(std::ostream& out, const HttpResponse& obj) {
    out << obj._version << " ";
    out << obj._statusCode << " ";
    out << obj._reason << " ";
    for(std::map<std::string, std::string>::iterator it = obj._headers.begin();\
    it != obj._headers.end(); it++) {
        out << it->first << ":" << it->second << std::endl;
    }
    return out;
}