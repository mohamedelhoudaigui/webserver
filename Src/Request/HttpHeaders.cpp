#include "../../Include/HttpHeaders.hpp"
#include <cctype>

const std::string HttpHeaders::VALID_TOKENS = "!#$%&'*+-.^_`|~";

bool HttpHeaders::isToken(char c) {
    return isalnum(c) || VALID_TOKENS.find(c) != std::string::npos;
}

bool HttpHeaders::isValidHeaderName(const std::string& name) {
    if (name.empty() || name.length() > MAX_HEADER_NAME_LENGTH)
        return false;
        
    // Header name must start with a letter
    if (!isalpha(name[0]))
        return false;
        
    // Check each character is valid token
    for (std::string::const_iterator it = name.begin(); it != name.end(); ++it) {
        if (!isToken(*it))
            return false;
    }
    
    return true;
}

bool HttpHeaders::isValidHeaderValue(const std::string& value) {
    if (value.length() > MAX_HEADER_VALUE_LENGTH)
        return false;
        
    // Check for control characters (except horizontal tab)
    for (std::string::const_iterator it = value.begin(); it != value.end(); ++it) {
        if (iscntrl(*it) && *it != '\t')
            return false;
    }
    
    return true;
}

bool HttpHeaders::isValidHeader(const std::string& header) {
    size_t colon = header.find(':');
    if (colon == std::string::npos)
        return false;
        
    std::string name = header.substr(0, colon);
    std::string value = header.substr(colon + 1);
    
    // Trim whitespace
    trim(value, " \t");
    
    return isValidHeaderName(name) && isValidHeaderValue(value);
}

bool HttpHeaders::isValidContentType(const std::string& value) {
    // Basic content type validation
    size_t slash = value.find('/');
    if (slash == std::string::npos)
        return false;
        
    std::string type = value.substr(0, slash);
    std::string subtype = value.substr(slash + 1);
    
    // Check for parameters
    size_t semicolon = subtype.find(';');
    if (semicolon != std::string::npos) {
        subtype = subtype.substr(0, semicolon);
        // Parameters validation could be added here
    }
    
    return !type.empty() && !subtype.empty();
}

bool HttpHeaders::isValidTransferEncoding(const std::string& value) {
    // HTTP/1.1 only supports 'chunked' and 'identity'
    std::string trimmed = value;
    trim(trimmed, " \t");
    return trimmed == "chunked" || trimmed == "identity";
}

bool HttpHeaders::isValidConnection(const std::string& value) {
    std::string trimmed = value;
    trim(trimmed, " \t");
    return trimmed == "keep-alive" || trimmed == "close";
}

bool HttpHeaders::validateRequiredHeaders(const std::map<std::string, std::string>& headers) {
    // Host is mandatory in HTTP/1.1
    std::map<std::string, std::string>::const_iterator host = headers.find("Host");
    if (host == headers.end() || !isValidHost(host->second))
        return false;
        
    // Validate Content-Length if present
    std::map<std::string, std::string>::const_iterator cl = headers.find("Content-Length");
    if (cl != headers.end() && !isValidContentLength(cl->second))
        return false;
        
    // Validate Content-Type if present
    std::map<std::string, std::string>::const_iterator ct = headers.find("Content-Type");
    if (ct != headers.end() && !isValidContentType(ct->second))
        return false;
        
    // Validate Transfer-Encoding if present
    std::map<std::string, std::string>::const_iterator te = headers.find("Transfer-Encoding");
    if (te != headers.end() && !isValidTransferEncoding(te->second))
        return false;
        
    return true;
}

std::string HttpHeaders::unfold(const std::string& value) {
    std::string result;
    bool last_was_space = false;
    
    for (std::string::const_iterator it = value.begin(); it != value.end(); ++it) {
        if (*it == '\r' || *it == '\n') {
            if (it + 1 != value.end() && (*(it + 1) == ' ' || *(it + 1) == '\t')) {
                last_was_space = true;
                ++it;
                continue;
            }
        }
        
        if (!last_was_space || (*it != ' ' && *it != '\t'))
            result += *it;
            
        last_was_space = (*it == ' ' || *it == '\t');
    }
    
    return result;
}
bool HttpHeaders::isValidContentLength(const std::string& value) {
    std::istringstream ss(value);
    size_t length;
    // Must be a valid and positive number
    for (size_t i = 0; i < value.length(); i++) {
        if (!isdigit(value[i]))
            return false;
    }
    // Convert and verify Valid positive number
    ss >> length;
    return !ss.fail() && length > 0;
}

bool HttpHeaders::isValidHost(const std::string &value) {
    //Could be a host name or ip:port
    size_t colonPosition = value.find(':');
    if (colonPosition != std::string::npos) {
        //validate port
        std::string port = value.substr(colonPosition + 1);
        for (size_t i = 0; i < port.length(); i++) {
            if (!isdigit(port[i]))
                return false;
        }
        std::istringstream ss(port);
        int portNum;
        ss >> portNum;
        if (portNum < 1 || portNum > 65535)
            return false;
    }
    return true;
}

std::string& HttpHeaders::trim(std::string &content, std::string needl) {
    content.erase(0, content.find_first_not_of(needl));
    content.erase(content.find_last_not_of(needl) + 1);
    return content;
}