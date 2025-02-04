#pragma once

#include <map>
#include <string>
#include <iostream>
#include <sstream>
#include <cstdlib>
#include "HttpHeaders.hpp"
#include "Router.hpp"
#include "UriValidator.hpp"
#include "Tools.hpp"
#include "Types.hpp"

class Request {
	protected:
		std::string							method;
		std::string							uri;
		std::string							http_version;
		std::string							query;
		std::string							path;
		std::map<std::string, std::string>	headers;
		std::string 						body;
		bool								chunked;
		size_t 								content_length;
		bool keep_alive;
		size_t max_keep_alive_requests;
		time_t keep_alive_timeout;
		time_t last_activity_time;
		
		virtual void parseRequestLine(const std::string& line);
		virtual void parseHeaders(std::istringstream& stream);
		virtual void parseUri();
		virtual void parseBody(std::istringstream& stream);
		virtual void parseChunkedBody(std::istringstream& stream);
		virtual void parseMultipartBody(std::istringstream& stream);
		virtual bool validateMethod(const std::string& method) const;
		virtual bool validateUri(const std::string& uri) const;
		virtual bool validateVersion(const std::string& version) const;
		virtual void processHeaders(const std::string& stream);
	public:

		Request();
		Request(const Request& req);
		virtual ~Request();
		const   std::string& getMethod() const;
		const   std::string& getUri() const;
		const   std::string& getHttpVersion() const;
		const   std::string& getQuery() const;
		const   std::string& getPath() const;
		const   std::string& getBody() const;
		const   std::map<std::string, std::string>& getHeaders() const;
		bool    isChunked() const;
		size_t	getContentLength() const;
		bool isKeepAlive() const;
		time_t getKeepAliveTimeout() const;
		size_t getMaxKeepAliveRequests() const;
		time_t getLastActivityTime() const;
		void updateLastActivityTime();
		void setKeepAlive(bool enabled);
		void setKeepAliveTimeout(time_t timeout);
		void setMaxKeepAliveRequests(size_t max_requests);
		virtual void reset();
		virtual void parseRequest(const std::string& raw_request);
};

class GET : public Request {
	public:
		GET();
		~GET();
		void parseRequestLine(const std::string& line);
		// void parseHeaders(std::istringstream& stream);
		// void parseUri();
		void parseBody(std::istringstream& stream);
		// void parseChunkedBody(std::istringstream& stream);
		// void parseMultipartBody(std::istringstream& stream);
		bool validateMethod(const std::string& method) const;
		// bool validateUri(const std::string& uri) const;
		// bool validateVersion(const std::string& version) const;
		// void reset();
};

class POST : public Request {
	private:
		bool multipart;
		std::string boundary;
		std::vector<std::string> parts;
		
	public:
		POST();
		~POST();
		void parseRequestLine(const std::string& line);
		void parseHeaders(std::istringstream& stream);
		void parseBody(std::istringstream& stream);
		void parseMultipartBody(std::istringstream& stream);
		bool validateMethod(const std::string& method) const;
		const std::vector<std::string>& getParts() const;
};

class DELETE : public Request {
	public:
		DELETE();
		~DELETE();
		void parseRequestLine(const std::string& line);
		bool validateMethod(const std::string& method) const;
		void validateUri() const;
		void parseBody(std::istringstream& stream);
};

//class Product

// class HttpRequest
// {
// 	private:
// 		std::string							method;
// 		std::string							uri;
// 		std::string							http_version;
// 		std::string							query;
// 		std::string							path;
// 		std::map<std::string, std::string>	headers;
// 		std::string 						body;
// 		bool								chunked;
// 		size_t 								content_length;
		
// 		bool    isValidMethod(const std::string& method) const;
// 		bool    isValidUri(const std::string& uri) const;
// 		bool    isValidVersion(const std::string& version) const;
// 		// private constructor can only be created throught builder
// 		HttpRequest();
// 		friend class HttpRequestBuilder;
// 	public:
// 		const   std::string& getMethod() const;
// 		const   std::string& getUri() const;
// 		const   std::string& getHttpVersion() const;
// 		const   std::string& getQuery() const;
// 		const   std::string& getPath() const;
// 		const   std::string& getBody() const;
// 		const   std::map<std::string, std::string>& getHeaders() const;
// 		bool    isChunked() const;
// 		size_t	getContentLength() const;
// 		~HttpRequest();
// };

// enum ReqParserState {
// 	REQUEST_LINE,
// 	HEADERS,
// 	BODY,
// 	COMPLETE,
// 	ERROR
// };

// // Builder class for constructing requests

// class HttpRequestBuilder {
// 	private:
// 		HttpRequest *request;
// 		bool	parseRequestLine(const std::string& line);
// 		bool	parseHeaders(std::istringstream& stream);
// 		// bool	parseBody(std::istringstream &stream);
// 		// bool	parseChunkedBody(std::istringstream& stream);
// 		bool	parseMultipartBody(std::istringstream& stream);
// 		void	parseUri();
// 		bool	validateMethod(const std::string& method) const;
// 		bool	validateUri(const std::string& uri) const;
// 		bool	validateVersion(const std::string& version) const;
// 		bool	validateContentLength(const std::string& length) const;
// 	public:
// 		HttpRequestBuilder();
// 		~HttpRequestBuilder();

// 		//Building methods
// 		HttpRequestBuilder& setMethod(const std::string& method);
// 		HttpRequestBuilder&	setUri(const std::string& uri);
// 		HttpRequestBuilder& setVersion(const std::string &version);
// 		HttpRequestBuilder& addHeader(const std::string& key, const std::string& value);
// 		HttpRequestBuilder& setBody(const std::string& body);

// 		//Parser
// 		bool parseRequest(const std::string& raw_request);
// 		HttpRequest* build();
// 		void	reset();
// };

// // Parser class for handling partial requests

// class HttpRequestParser {
// 	private:
// 		HttpRequestBuilder builder;
// 		std::string buffer;
// 		ReqParserState state;
// 		size_t max_size;
// 		size_t max_request_size;
// 		size_t parsePosition;  // Track parsing position
// 		size_t bodyBytesRead;  // Track body bytes read
		
// 		bool parseRequestLine();
// 		bool parseHeaders();
// 		bool parseBody();
// 		bool parseChunkedBody();

// 	public:
// 		HttpRequestParser(size_t max_size);
// 		bool	feed(const std::string& data);
// 		bool	isComplete() const;
// 		HttpRequest* getRequest();
// 		void	reset();
// };

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
