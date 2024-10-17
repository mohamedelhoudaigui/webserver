#pragma once

#include <iostream>
#include <map>
#include <string>
#include <vector>
/*
This implementation allows to:
        Create a basic response
        Modify it with various decorators to add headers and change behavior
        Generate the final response string
*/
class Response {
        public:
                virtual std::string getStatusLine() const = 0;
                virtual std::map<std::string, std::string> getHeaders() const = 0;
                virtual std::string getBody() const = 0;
                virtual ~Response();
};
//A concrete implementation of a basic HTTP response.
class BasicResponse : public Response {
        private:
                std::map<std::string, std::string> headers;
                std::string     statusText;
                std::string     body;
                int             statusCode;
        public:
                BasicResponse(int code, std::string& text);
                std::string     getStatusLine() const;
                std::map<std::string, std::string> getHeaders() const;
                std::string getBody() const;
                void setBody(const std::string& newBody);
};
//The base decorator class that wraps an HttpResponse.
class ResponseDecorator : public Response {
        protected:
                Response*       response;
        public:
                ResponseDecorator(Response* res);
                std::string     getStatusLine() const;
                std::map<std::string, std::string> getHeaders() const;
                std::string getBody() const;
                virtual ~ResponseDecorator();
};
/*
Specific decorators:
        ContentTypeDecorator: Adds Content-Type header
        CachingDecorator: Adds Cache-Control header
        CompressionDecorator: Adds Content-Encoding header (and in a real implementation would compress the body)
        // CorsDecorator: Adds Access-Control-Allow-Origin header
        // SecurityHeadersDecorator: Adds various security-related headers
        CookieDecorator: Adds Set-Cookie headers  
*/
class ContentTypeDecorator : public ResponseDecorator { 
        private:
                std::string     contentType;
        public:
                ContentTypeDecorator(Response* res, const std::string& type);
                std::map<std::string, std::string> getHeaders() const;
};

class CachingDecorator : public ResponseDecorator {
        private:
                std::string cacheControl;

        public:
                CachingDecorator(Response* r, const std::string& control);
                std::map<std::string, std::string> getHeaders() const;
};

class CompressionDecorator : public ResponseDecorator {
        private:
                std::string encodingType;

        public:
                CompressionDecorator(Response* r, const std::string& encoding);
                std::map<std::string, std::string> getHeaders() const;
                std::string getBody() const;
};

class CookieDecorator : public ResponseDecorator {
        private:
        std::vector<std::string> cookies;

        public:
        CookieDecorator(Response* r);
        void addCookie(const std::string& name, const std::string& value, 
                        const std::string& path = "/", int maxAge = -1, 
                        bool httpOnly = true, bool secure = true);
        std::map<std::string, std::string> getHeaders() const;
};

std::string buildFullResponse(const Response* response);
std::string getCurrentTime();
