#include "../../Include/Response/Response.hpp"
#include <ctime>
#include <sstream>

std::string getCurrentTime() {
    time_t now = time(0);
    struct tm* tm_info = gmtime(&now);
    char buffer[80];
    strftime(buffer, 80, "%a, %d %b %Y %H:%M:%S GMT", tm_info);
    return std::string(buffer);
}

BasicResponse::BasicResponse(int code, const std::string& text) : \
statusCode(code), statusText(text) {
    headers["Server"] = "Nginx";
    headers["Date"] = getCurrentTime();
}

std::string BasicResponse::getStatusLine() const {
    std::stringstream ss;
    ss << "HTTP/1.1" << statusCode << " " << statusText;
    return ss.str();
}

std::map<std::string, std::string> BasicResponse::getHeaders() const {
    return headers;
}

std::string BasicResponse::getBody() const {
    return body;
}

void    BasicResponse::setBody(const std::string& newBody) {
    std::stringstream ss;
    body = newBody;
    ss << body.length();
    headers["Content-Length"] = ss.str();
}

ResponseDecorator::ResponseDecorator(Response* res) : response(res) {

};

ResponseDecorator::~ResponseDecorator() {
    delete response;
}

std::map<std::string, std::string> ResponseDecorator::getHeaders() const {
    return response->getHeaders();
}

std::string  ResponseDecorator::getStatusLine() const {
    return response->getStatusLine();
}

std::string ResponseDecorator::getBody() const {
    return response->getBody();
}

ContentTypeDecorator::ContentTypeDecorator(Response* res, const std::string& type) :\
ResponseDecorator(res), contentType(type) {

}

