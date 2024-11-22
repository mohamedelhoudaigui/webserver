#include "../../Include/HttpHeaders.hpp"
#include <cctype>

bool HTTPHeader::isValidFieldName(const std::string& name) {
    if (name.empty()) return false;
    
    // First char must be letter
    if (!isalpha(name[0])) return false;
    
    // Check rest of characters
    for (size_t i = 1; i < name.length(); ++i) {
        char c = name[i];
        if (!isalpha(c) && !isdigit(c) && c != '-') return false;
    }
    return true;
}

bool HTTPHeader::isValidFieldValue(const std::string& value) {
    // Check for printable characters
    for (size_t i = 0; i < value.length(); ++i) {
        if (!isprint(value[i]) && !isspace(value[i])) return false;
    }
    return true;
}

bool HTTPHeader::isValidHeader(const std::string& line) {
    size_t colonPos = line.find(':');
    if (colonPos == std::string::npos) 
        return false;
    // Extract field name and value
    std::string name = line.substr(0, colonPos);
    std::string value = line.substr(colonPos + 1);
    // Trim value
    value.erase(0, value.find_first_not_of(" \t"));
    value.erase(value.find_last_not_of(" \t\r\n") + 1);
    return isValidFieldName(name) && isValidFieldValue(value);
}