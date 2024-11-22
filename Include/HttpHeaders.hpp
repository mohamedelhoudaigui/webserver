#ifndef HTTP_HEADER_HPP
#define HTTP_HEADER_HPP


#include <string>

class HTTPHeader {
private:
    static bool isValidFieldName(const std::string& name);
    static bool isValidFieldValue(const std::string& value);

public:
    static bool isValidHeader(const std::string& line);
};

#endif

/*
    Rules for field-name:
        - Must start with a letter (A-Z, a-z)
        - Can contain letters, digits, hyphen
        - Cannot contain spaces
        - Case-insensitive
        - No special characters except hyphen

        Valid examples:
        Content-Type: text/html
        Accept: *'/*
        X-Custom-Header: value

        Invalid examples:
        Content Type: text/html    (space in name)
        @Accept: *'/*              (special character)
        : value                   (no name)
*/

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