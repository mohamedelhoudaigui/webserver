#pragma once

#include <map>
#include <string>
#include <iostream>
#include <sstream>
#include "HttpHeaders.hpp"

//class Product

class HttpRequest
{
	private:
		std::string							method;
		std::string							uri;
		std::string							http_version;
		std::string							query;
		std::string							path;
		std::map<std::string, std::string>	headers;
		std::string 						body;
		bool								chunked;
		size_t 								content_length;
		
		bool    isValidMethod(const std::string& method) const;
		bool    isValidUri(const std::string& uri) const;
		bool    isValidVersion(const std::string& version) const;
		// private constructor can only be created throught builder
		HttpRequest();
		friend class HttpRequestBuilder;
	public:
		const   std::string& getMethod() const;
		const   std::string& getUri() const;
		const   std::string& getHttpVersion() const;
		const   std::string& getQuery() const;
		const   std::string& getPath() const;
		const   std::string& getBody() const;
		const   std::map<std::string, std::string>& getHeaders() const;
		bool    isChunked() const;
		size_t	getContentLength() const;
		~HttpRequest();
};

enum ReqParserState {
	REQUEST_LINE,
	HEADERS,
	BODY,
	COMPLETE,
	ERROR
};

// Builder class for constructing requests

class HttpRequestBuilder {
	private:
		HttpRequest *request;
		bool	parseRequestLine(const std::string& line);
		bool	parseHeaders(std::istringstream& stream);
		// bool	parseBody(std::istringstream &stream);
		// bool	parseChunkedBody(std::istringstream& stream);
		bool	parseMultipartBody(std::istringstream& stream);
		void	parseUri();
		bool	validateMethod(const std::string& method) const;
		bool	validateUri(const std::string& uri) const;
		bool	validateVersion(const std::string& version) const;
		bool	validateContentLength(const std::string& length) const;
	public:
		HttpRequestBuilder();
		~HttpRequestBuilder();

		//Building methods
		HttpRequestBuilder& setMethod(const std::string& method);
		HttpRequestBuilder&	setUri(const std::string& uri);
		HttpRequestBuilder& setVersion(const std::string &version);
		HttpRequestBuilder& addHeader(const std::string& key, const std::string& value);
		HttpRequestBuilder& setBody(const std::string& body);

		//Parser
		bool parseRequest(const std::string& raw_request);
		HttpRequest* build();
		void	reset();
};

// Parser class for handling partial requests

class HttpRequestParser {
	private:
		HttpRequestBuilder builder;
		std::string buffer;
		ReqParserState state;
		size_t max_size;
		size_t max_request_size;

	public:
		HttpRequestParser(size_t max_size);
		bool	feed(const std::string& data);
		bool	isComplete() const;
		HttpRequest* getRequest();
		void	reset();
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
	Body
*/