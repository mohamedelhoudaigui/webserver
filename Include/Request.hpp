#ifndef REQUEST_HPP
#define REQUEST_HPP

#include <sstream>
#include <cctype>
#include <vector>
#include <algorithm>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>


#include "Tools.hpp"
#include "Types.hpp"


class Request
{
    public:
        Request();
        int             ReadBuffer(struct epoll_event event);
        void            Parse(std::stringstream& ss);
        void            ParseReqline(std::stringstream& ss);
        void            ParseHeaders(std::stringstream& ss);
        void            ParseBody(std::stringstream& ss);
        void            ParseQueryString(std::string& req_target);
        bool            IsValidHeaderKeyChar(char c);
        void            ParseHeader(std::string& header_string);
        void            SetResultTrue(unsigned int code, std::string comment);
        std::vector<std::string>    GetTokens(std::string& line, char seperator, int n);
        void            print_info();

        std::string Method;
        std::string Path;
        std::string Version;
        std::map<std::string, std::string>  querys;
        std::map<std::string, std::string>  headers;
        std::string                         body;
        bool                                finished;
        unsigned int                        status;
};




#endif
