#include "../../Include/HttpRequest.hpp"

Request::Request() 
    : chunked(false)
    , content_length(0)
    , keep_alive(true)  // HTTP/1.1 defaults to keep-alive
    , max_keep_alive_requests(100)
    , keep_alive_timeout(5)  // 5 seconds default timeout
    , last_activity_time(time(NULL))
{}

Request::~Request() {}

const std::string& Request::getContentType() const {
      std::map<std::string, std::string>::const_iterator it = headers.find("Content-Type");
    if (it != headers.end()) {
        return it->second;
    } else {
        return "";
    }
}

void Request::reset() {
    method.clear();
    uri.clear();
    http_version.clear();
    query.clear();
    path.clear();
    headers.clear();
    body.clear();
    chunked = false;
    content_length = 0;
}

void Request::parseRequest(const std::string& raw_request) {
    Logger(INFO, "starting to parse request");//------logger

    std::istringstream stream(raw_request);
    std::string request_line;

    if (!std::getline(stream, request_line))
        throw std::runtime_error("Empty request");

    if (request_line[request_line.length() - 1] == '\r')
        request_line.erase(request_line.length() - 1);

    parseRequestLine(request_line);
    Logger(INFO, "parsed request line : " + method + " " + uri + " " + http_version);
    parseHeaders(stream);
    Logger(INFO, "parsed headers");//------logger
    if (chunked)
        parseChunkedBody(stream);
    else if (content_length > 0)
        parseBody(stream);
}

void Request::parseRequestLine(const std::string& line) {
    std::string::size_type first_space = line.find(' ');
    std::string::size_type last_space = line.rfind(' ');

    if (first_space == std::string::npos || last_space == std::string::npos || first_space == last_space)
        throw std::runtime_error("Invalid request line format");

    method = line.substr(0, first_space);
    uri = line.substr(first_space + 1, last_space - first_space - 1);
    http_version = line.substr(last_space + 1);
    if (!validateMethod(method) || !validateUri(uri) || !validateVersion(http_version))
        throw std::runtime_error("Invalid request line components");

    parseUri();
}

void Request::parseUri() {
    if (uri.length() > UriValidator::MAX_URI_LENGTH)
        throw std::runtime_error("URI too long");

    std::string::size_type query_pos = uri.find('?');
    std::string::size_type fragment_pos = uri.find('#');

    // Extract path
    if (query_pos != std::string::npos) {
        path = uri.substr(0, query_pos);
        query = uri.substr(query_pos + 1, 
                         (fragment_pos != std::string::npos) ? 
                         fragment_pos - query_pos - 1 : std::string::npos);
    } else {
        path = uri.substr(0, fragment_pos);
        query.clear();
    }

    // Validate and sanitize path
    if (!UriValidator::isValidPath(path))
        throw std::runtime_error("Invalid path format");

    path = UriValidator::normalizePath(UriValidator::percentDecode(path));

    // Validate query string if present
    // if (!query.empty() && !UriValidator::isValidQuery(query))
    //     throw std::runtime_error("Invalid query string format");
}

void Request::parseHeaders(std::istringstream& stream) {
    std::string line;
    std::string current_header;
    size_t header_count = 0;
    bool in_folded_header = false;

    while (std::getline(stream, line) && !line.empty() && line != "\r")
    {
        if (header_count >= MAX_HEADER_COUNT)
            throw std::runtime_error("Too many headers");

        if (line[line.length() - 1] == '\r')
            line.erase(line.length() - 1);

        // Handle header folding
        if (line[0] == ' ' || line[0] == '\t') {
            if (!in_folded_header)
                throw std::runtime_error("Invalid header folding");
            current_header += "\r\n" + line;
            continue;
        }
 
        // Process previous header if exists
        if (in_folded_header) {
            processHeaders(HttpHeaders::unfold(current_header));
            current_header.clear();
        }
 
        current_header = line;
        Logger(INFO, "current header : " + current_header);//------logger
        in_folded_header = true;
        header_count++;
    }
 
    // Process last header
    if (in_folded_header)
        processHeaders(HttpHeaders::unfold(current_header));
    // Validate all required headers are present and valid
    if (!HttpHeaders::validateRequiredHeaders(headers))
        throw std::runtime_error("Missing or invalid required headers");
}

void Request::processHeaders(const std::string& header) {
    if (!HttpHeaders::isValidHeader(header))
        throw std::runtime_error("Invalid header format: " + header);
 
    std::string::size_type colon = header.find(':');
    if (colon == std::string::npos)
        throw std::runtime_error("Invalid header format");

    std::string key = header.substr(0, colon);
    std::string value = header.substr(colon + 1);
    HttpHeaders::trim(value, " \t");
 
    // Special header processing
    if (key == "Content-Length") {
        if (!HttpHeaders::isValidContentLength(value))
            throw std::runtime_error("Invalid Content-Length header");
        content_length = std::atol(value.c_str());
        if (content_length > MAX_BODY_SIZE)
            throw std::runtime_error("Content-Length exceeds maximum allowed size");
    }
    else if (key == "Transfer-Encoding") {
        if (!HttpHeaders::isValidTransferEncoding(value))
            throw std::runtime_error("Unsupported Transfer-Encoding");
        chunked = (value == "chunked");
        if (chunked && headers.find("Content-Length") != headers.end())
            throw std::runtime_error("Content-Length not allowed with Transfer-Encoding: chunked");
    }
    else if (key == "Content-Type") {
        if (!HttpHeaders::isValidContentType(value))
            throw std::runtime_error("Invalid Content-Type header");
    }
    else if (key == "Host") {
        if (!HttpHeaders::isValidHost(value))
            throw std::runtime_error("Invalid Host header");
    }
    else if (key == "Connection") {
        if (HttpHeaders::trim(value, " \t") == "close") {
            keep_alive = false;
        } else if (HttpHeaders::trim(value, " \t") == "keep-alive") {
            keep_alive = true;
        }
    }
    else if (key == "Keep-Alive") {
        // Parse keep-alive parameters
        std::string::size_type pos = value.find("timeout=");
        if (pos != std::string::npos) {
            std::string timeout_str = value.substr(pos + 8);
            keep_alive_timeout = std::atoi(timeout_str.c_str());
        }
 
        pos = value.find("max=");
        if (pos != std::string::npos) {
            std::string max_str = value.substr(pos + 4);
            max_keep_alive_requests = std::atoi(max_str.c_str());
        }
    }
 
    // Check for duplicate headers
    if (headers.find(key) != headers.end())
        throw std::runtime_error("Duplicate header: " + key);

    headers[key] = value;
    Logger(DEBUG, "header stored : " + key + "=" + headers[key]);//------logger
}

void Request::parseBody(std::istringstream& stream)
{
    Logger(INFO, "normal body parse");//-------logger
    char* buffer = new char[content_length + 1];
    stream.read(buffer, content_length);
    buffer[content_length] = '\0'; // -- ISSUE HERE ! : body parts can be binary !
    body.assign(buffer, content_length);
    delete[] buffer;
}

void Request::parseChunkedBody(std::istringstream& stream) {
    Logger(INFO, "chunked body parse");//-------logger
    std::string chunk_size_str;
    size_t total_size = 0;

    while (std::getline(stream, chunk_size_str)) {
        if (chunk_size_str[chunk_size_str.length() - 1] == '\r')
            chunk_size_str.erase(chunk_size_str.length() - 1);
 
        // Convert hex string to size_t
        size_t chunk_size;
        std::istringstream hex_stream(chunk_size_str);
        hex_stream >> std::hex >> chunk_size;
 
        if (chunk_size == 0)
            break;
 
        if (total_size + chunk_size > MAX_BODY_SIZE)
            throw std::runtime_error("Body too large");
 
        char* chunk = new char[chunk_size + 1];
        stream.read(chunk, chunk_size);
        chunk[chunk_size] = '\0';
 
        body.append(chunk, chunk_size);
        delete[] chunk;
 
        total_size += chunk_size;
 
        // Skip CRLF
        stream.ignore(2);
    }
}

void Request::parseMultipartBody(std::istringstream& stream) {
    std::string boundary = headers["Content-Type"];
    size_t pos = boundary.find("boundary=");
    if (pos == std::string::npos)
        throw std::runtime_error("No boundary in multipart content");

    boundary = "--" + boundary.substr(pos + 9);
    std::string line;

    while (std::getline(stream, line)) {
        if (line[line.length() - 1] == '\r')
            line.erase(line.length() - 1);

        if (line == boundary || line == boundary + "--")
            continue;

        // Parse part headers and content
        // This is a simplified version - real implementation would need to handle
        // Content-Disposition, Content-Type, etc.
        if (line.empty())
            continue;
    }
}

bool Request::validateMethod(const std::string& method) const {
    return method == "GET" || method == "POST" || method == "DELETE";
}

bool Request::validateUri(const std::string& uri) const {
    return !uri.empty() && uri[0] == '/' && UriValidator::isValidPath(uri);
}

bool Request::validateVersion(const std::string& version) const {
    return version == "HTTP/1.1";
}

bool Request::isKeepAlive() const {
    return keep_alive;
}

time_t Request::getKeepAliveTimeout() const {
    return keep_alive_timeout;
}

size_t Request::getMaxKeepAliveRequests() const {
    return max_keep_alive_requests;
}

time_t Request::getLastActivityTime() const {
    return last_activity_time;
}

void Request::updateLastActivityTime() {
    last_activity_time = time(NULL);
}

void Request::setKeepAlive(bool enabled) {
    keep_alive = enabled;
}

void Request::setKeepAliveTimeout(time_t timeout) {
    keep_alive_timeout = timeout;
}

void Request::setMaxKeepAliveRequests(size_t max_requests) {
    max_keep_alive_requests = max_requests;
} 


const std::string& Request::getMethod() const {
    return method;
}

const std::string& Request::getUri() const {
    return uri;
}

const std::string& Request::getHttpVersion() const {
    return http_version;
}

const std::string& Request::getQuery() const {
    return query;
}

const std::string& Request::getPath() const {
    return path;
}

const std::string& Request::getBody() const {
    return body;
}

const std::map<std::string, std::string>& Request::getHeaders() const {
    return headers;
}

bool Request::isChunked() const {
    return chunked;
}

size_t Request::getContentLength() const {
    return content_length;
}
