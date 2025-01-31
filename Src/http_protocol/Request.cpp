#include "../../Include/Request.hpp"


Request::Request()
{
    finished = false;
    status = INIT;
}

int    Request::ReadBuffer(struct epoll_event event)
{
    char buffer[BUFFER_SIZE];
    int bytes_received = recv(event.data.fd, buffer, sizeof(buffer), 0);

    if (bytes_received <= 0)
    {
        if (status == PENDING)
            SetResultTrue(INVALID, "body smaller than content-length");
        return (-1);
    }
    std::string buffer_string(buffer, bytes_received);
    std::stringstream ss(buffer_string);
    Parse(ss);
    return (0);
}

void    Request::Parse(std::stringstream& ss)
{
    switch (status)
    {
        case INIT: // new fresh request
            ParseReqline(ss);
            CHECK_ERROR(finished, INVALID, "error parsing request line");
            ParseHeaders(ss);
            CHECK_ERROR(finished, INVALID, "error parsing headers");
            CHECK_ERROR(headers.find(HOST) == headers.end(), INVALID, "host header not found");
            ParseBody(ss);
            break;

        case PENDING: // buffering body
            ParseBody(ss);
            break;
    }
}

void    Request::ParseReqline(std::stringstream& ss)
{
    std::string line;
    std::getline(ss, line);

    // check for \r\n :
    CHECK_ERROR(line[line.size() - 1] != HTTP_DELIM, INVALID, "request line doesnt end with HTTP_DELIM");

    // remove them:
    line = TrimAll(line);
    std::vector<std::string> tokens = GetTokens(line, ' ', 3);
    CHECK_ERROR(finished, INVALID, "request line has more than 3 tokens");

    // -- parse method:
    CHECK_ERROR((tokens[0] != "GET"
                && tokens[0] != "POST"
                && tokens[0] != "DELETE"
                && tokens[0] != "PUT"),
            INVALID, "invalid method");
    Method = tokens[0];

    // -- parse request target:
    CHECK_ERROR(tokens[1][0] != '/', INVALID, "request target doesnt start with backslash");
    if (tokens[1].find('?') != std::string::npos)
    {
        std::string req_target = tokens[1].substr(0, tokens[1].find('?'));
        std::string query_string = tokens[1].substr(tokens[1].find('?') + 1, tokens[1].size() - 1);
        Path = req_target;
        ParseQueryString(query_string);
    }
    else
        Path = tokens[1];
    CHECK_ERROR(finished, INVALID, "query variable error");

    // -- parse http version
    CHECK_ERROR(tokens[2] != HTTP_VERSION, INVALID, "invalid http version"); 
    Version = HTTP_VERSION;
}

void    Request::ParseQueryString(std::string& req_target)
{
    std::stringstream s(req_target);
    std::string     token;

    while (std::getline(s, token, '&'))
    {
        size_t  pos = token.find('=');
        if (pos == std::string::npos)
        {
            finished = true;
            return ;
        }
        std::string key = token.substr(0, pos);
        std::string value = token.substr(pos + 1, token.size() - 1);
        querys[key] = value;
    }
}

void    Request::ParseHeaders(std::stringstream& ss)
{
    std::string header_line;
    size_t i = 0;

    while (std::getline(ss, header_line))
    {
        if (header_line[0] == HTTP_DELIM)
            break ;
        CHECK_ERROR(i == MAX_HEADER_NUM, INVALID, "numbers of headers exeeded");
        CHECK_ERROR(header_line[header_line.size() - 1] != HTTP_DELIM, INVALID, "header doesnt end with HTTP_DELIM");
        CHECK_ERROR(header_line.size() > MAX_HEADER_LEN, INVALID, "header field limit exeeded");

        header_line = TrimAll(header_line);
        ParseHeader(header_line);
        if (finished)
            break ;
        ++i;
    }
}

bool Request::IsValidHeaderKeyChar(char c)
{
    return (std::isalnum(c) || c == '-');
}

void    Request::ParseHeader(std::string& header_string)
{
    CHECK_ERROR(header_string.size() >= MAX_HEADER_LEN, INVALID, "header with size > 256")
    size_t pos = header_string.find(':');
    CHECK_ERROR(pos == std::string::npos, INVALID, "found header with no seperator")
    std::string key = header_string.substr(0, pos);
    std::string value = header_string.substr(pos + 1, header_string.size() -1);
    bool valid_key = true;
    for (int i = 0; i < key.size(); ++i)
    {
        if (!IsValidHeaderKeyChar(key[i]))
        {
            valid_key = false;
            break;
        }
        key[i] = tolower(key[i]);
    }
    CHECK_ERROR(!valid_key, INVALID, "invalid character in header key");
    headers[TrimAll(key)] = TrimAll(value);
}

void Request::ParseBody(std::stringstream& ss)
{
    if (headers.find(CONT_LEN) == headers.end())
    {
        SetResultTrue(OK, "no content-length, no body parsed");
        return;
    }
    // Append all remaining data in the stream to the body
    std::string body_part;
    body_part.assign(
        std::istreambuf_iterator<char>(ss),
        std::istreambuf_iterator<char>()
    );
    body += body_part;

    unsigned int content_len = std::atoi(headers[CONT_LEN].c_str());
    if (content_len == body.size()) {
        status = OK;
    } else if (content_len > body.size()) {
        status = PENDING;
    } else {
        SetResultTrue(INVALID, "body size exceeds content-length");
    }
}

std::vector<std::string>    Request::GetTokens(std::string& line, char seperator, int n)
{
    std::vector<std::string>    res;
    std::string                 token;
    std::stringstream          s(line);

    while (std::getline(s, token, seperator))
        res.push_back(token);
    if (res.size() != n)
        finished = true;
    return (res);
}

void    Request::SetResultTrue(unsigned int code, std::string comment)
{
    finished = true;
    status = code;
    Logger(DEBUG, comment);
}


void    Request::print_info()
{
    std::cout << RED << "request line :" << RESET << std::endl;
    std::cout << Method
    << " " << Path
    << " " << Version
    << std::endl;

    std::cout << BLUE << "query parameters :" << RESET << std::endl;
    if (querys.size() == 0)
        std::cout << "empty" << std::endl;
    else
    {
        std::map<std::string, std::string>::iterator it;
        for (it = querys.begin(); it != querys.end(); ++it)
            std::cout << it->first << " = " << it->second << std::endl;
    }
    std::cout << GREEN << "http headers :" << RESET << std::endl;
    if (headers.size() == 0)
        std::cout << "empty" << std::endl;
    else
    {
        std::map<std::string, std::string>::iterator it;
        for (it = headers.begin(); it != headers.end(); ++it)
        {
            std::cout << it->first
            << "=" << it->second
            << std::endl;
        }
    }
    std::cout << YELLOW << "body :" << RESET << std::endl;
    if (body.empty())
        std::cout << "empty" << std::endl;
    else
    {
        std::cout << body << std::endl;
    }
}
