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

/*
valid headers Format:


field-name ":" [ field-value ]
Valid examples:
Content-Type: text/html
Accept: * / *
X-Custom-Header: value


Invalid examples:
Content Type: text/html    (space in name)
@Accept: *'/*              (special character)
: value                   (no name)


field-name:
     (first char must be letter and the rest can be letters, digits, teri b 6) spaces are forbidden
    Case insensitive
field-value:

*/
// #pragma once

// #include <string>
// #include <iostream>
// #include <sstring>

// # include "HttpHeaders.hpp"

// #define METHOD 0x01  // single space after
// #define PATH 0x02 //single space after
// #define VERSION 0x03 // \r\n
// #define HEADER 0x04 // empty line after

// class HttpParser {

//     private:
//         std::string requestLine;
//         std::
// }
// username=john&age=25