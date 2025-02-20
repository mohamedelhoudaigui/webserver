#include "../../Include/UriValidator.hpp"
#include <algorithm>
#include <sstream>

const std::string UriValidator::RESERVED_CHARS = ":/?#[]@!$&'()*+,;=";
const std::string UriValidator::UNRESERVED_CHARS = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789-._~";

bool UriValidator::isHexDigit(char c) {
    return isdigit(c) || (c >= 'A' && c <= 'F') || (c >= 'a' && c <= 'f');
}

int UriValidator::hexToInt(char c) {
    if (c >= '0' && c <= '9') return c - '0';
    if (c >= 'A' && c <= 'F') return c - 'A' + 10;
    if (c >= 'a' && c <= 'f') return c - 'a' + 10;
    return 0;
}

std::string UriValidator::percentDecode(const std::string& str) {
    std::string result;
    for (size_t i = 0; i < str.length(); ++i) {
        if (str[i] == '%' && i + 2 < str.length() && 
            isHexDigit(str[i + 1]) && isHexDigit(str[i + 2])) {
            char decoded = (hexToInt(str[i + 1]) << 4) | hexToInt(str[i + 2]);
            result += decoded;
            i += 2;
        } else {
            result += str[i];
        }
    }
    return result;
}

bool UriValidator::isValidPath(const std::string& path) {
    if (path.empty() || path[0] != '/' || path.length() > MAX_URI_LENGTH)
        return false;

    std::string decoded = percentDecode(path);
    if (hasTraversalAttempt(decoded))
        return false;

    for (std::string::const_iterator it = path.begin(); it != path.end(); ++it) {
        if (*it != '%' && *it != '/' && 
            UNRESERVED_CHARS.find(*it) == std::string::npos &&
            RESERVED_CHARS.find(*it) == std::string::npos)
            return false;
    }
    return true;
}

std::string UriValidator::normalizePath(const std::string& path) {
    std::string normalized;
    std::istringstream stream(path);
    std::string segment;
    std:: vector<std::string> segments;

    while (std::getline(stream, segment, '/')) {
        if (segment == "." || segment.empty())
            continue;
        if (segment == "..") {
            if (!segments.empty())
                segments.pop_back();
        } else {
            segments.push_back(segment);
        }
    }

    for (std::vector<std::string>::const_iterator it = segments.begin(); 
         it != segments.end(); ++it) {
        normalized += "/" + *it;
    }
    
    return normalized.empty() ? "/" : normalized;
}

bool UriValidator::hasTraversalAttempt(const std::string& path) {
    return path.find("../") != std::string::npos || 
           path.find("./") != std::string::npos ||
           path.find("//") != std::string::npos;
} 