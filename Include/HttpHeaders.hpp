#ifndef HTTP_HEADERS_HPP
#define HTTP_HEADERS_HPP

#include <string>
#include <map>
#include <vector>
#include <cctype>
#include <sstream>

class HttpHeaders {
    private:
        static const size_t MAX_HEADER_NAME_LENGTH = 256;
        static const size_t MAX_HEADER_VALUE_LENGTH = 4096;
        static const std::string VALID_TOKENS;
        
        static bool isToken(char c);
        static bool isValidHeaderName(const std::string& name);
        static bool isValidHeaderValue(const std::string& value);
        static bool isFolded(const std::string& line);

    public:
        static bool isValidHeader(const std::string& header);
        static bool isValidHost(const std::string& value);
        static bool isValidContentLength(const std::string& value);
        static bool isValidContentType(const std::string& value);
        static bool isValidTransferEncoding(const std::string& value);
        static bool isValidConnection(const std::string& value);
        static std::string& trim(std::string& content, std::string needl);
        static std::string unfold(const std::string& value);
        
        // New method to validate all required headers for HTTP/1.1
        static bool validateRequiredHeaders(const std::map<std::string, std::string>& headers);
};

#endif




    // Rules for field-name:
    //     - Must start with a letter (A-Z, a-z)
    //     - Can contain letters, digits, hyphen
    //     - Cannot contain spaces
    //     - Case-insensitive
    //     - No special characters except hyphen

    //     Valid examples:
    //     Content-Type: text/html
    //     //Accept: */*
    //     X-Custom-Header: value

    //     Invalid examples:
    //     Content Type: text/html    (space in name)
    //     @Accept: *'/*              (special character)
    //     : value                   (no name)


/*
    Rules for field-value:
        - Can be empty
        - Can contain any printable characters
        - Leading/trailing whitespace should be trimmed
        - Multiple values separated by commas
        - Line folding (continuation) with spaces/tabs

        Valid examples:
        Accept: text/html, application/xml
        Server: Apache,
                Nginx
        Content-Type: text/html

        Invalid examples:
        Content-Type: text/html\0   (null character)
        Server: Apache\n            (newline without space/tab)
*/