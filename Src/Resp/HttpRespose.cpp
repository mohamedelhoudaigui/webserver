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
