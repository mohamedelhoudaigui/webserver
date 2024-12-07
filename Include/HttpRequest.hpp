#pragma once

#include <map>
#include <string>
#include <iostream>
#include <sstream>
#include "HttpHeaders.hpp"

//TODO

class HttpRequest
{
	private:
		std::string							method;
		std::string							uri;
		std::string							http_version;
		std::map<std::string, std::string>	headers;
		std::string							body;
	
		bool    isValidMethod(const std::string& method) const;
		bool    isValidUri(const std::string& uri) const;
		bool    isValidVersion(const std::string& version) const;
	public:
		HttpRequest(std::string& RawRequest);
		~HttpRequest();

		void	InitParse() const;
		bool    Parser(const std::string& line);
		void    parseUri(std::string &path, std::string &query) const;

		const   std::string& getMethod() const;
		const   std::string& getUri() const;
		const   std::string& getHttpVersion() const;
};

// mel-houd edit:
// added raw request to constructor
// added std::string body to buffer body



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