#pragma once

#include <string>
#include <map>
#include <vector>

class UriValidator {
private:
    
    static bool isHexDigit(char c);
    static int hexToInt(char c);

public:
    static const size_t MAX_URI_LENGTH = 2048;
    static const std::string RESERVED_CHARS;
    static const std::string UNRESERVED_CHARS;
    static std::string percentDecode(const std::string& str);
    static bool isValidScheme(const std::string& scheme);
    static bool isValidPath(const std::string& path);
    // static bool isValidQuery(const std::string& query);
    static bool isValidFragment(const std::string& fragment);
    static std::string sanitizePath(const std::string& path);
    static std::string normalizePath(const std::string& path);
    static bool hasTraversalAttempt(const std::string& path);
};

