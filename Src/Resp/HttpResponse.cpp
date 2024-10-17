#include "../../Include/Response/Response.hpp"

std::string getCurrentTime() {
    time_t now = time(0);
    struct tm* tm_info = gmtime(&now);
    char buffer[80];
    strftime(buffer, 80, "%a, %d %b %Y %H:%M:%S GMT", tm_info);
    return std::string(buffer);
}