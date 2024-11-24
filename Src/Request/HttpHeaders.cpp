#include "../../Include/HttpHeaders.hpp"
#include <cctype>

bool HttpHeaders::isValidFieldKey(const std::string& key) {
    //return false if the key is empty
    if (key.empty()) 
        return false;
    // First char must be letter
    if (!isalpha(key[0])) 
        return false;
    // Check rest of characters
    for (size_t i = 1; i < key.length(); ++i) {
        char c = key[i];
        if (!isalpha(c) && !isdigit(c) && c != '-') 
            return false;
    }
    return true;
}

bool HttpHeaders::isValidFieldValue(const std::string& value) {
    // Check for printable characters
    for (size_t i = 0; i < value.length(); ++i) {
        if (!isprint(value[i]) && !isspace(value[i])) 
            return false;
        if ((value[i] == '\n' && i + 1 != std::string::npos) && (value[i + 1] != '\t' || value[i + 1] != ' '))
            return false;
    }
    return true;
}

bool HttpHeaders::isValidHeader(const std::string& line) {
    size_t colonPosition = line.find(':');
    if (colonPosition == std::string::npos) 
        return false;
    // Extract field key and value
    std::string key = line.substr(0, colonPosition);
    std::string value = line.substr(colonPosition + 1);
    // Trim value
    value.erase(0, value.find_first_not_of(" \t"));
    value.erase(value.find_last_not_of(" \t\r\n") + 1);
    return isValidFieldKey(key) && isValidFieldValue(value);
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