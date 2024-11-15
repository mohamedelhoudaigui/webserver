/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   core_request.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mboutuil <mboutuil@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/12 11:46:55 by mboutuil          #+#    #+#             */
/*   Updated: 2024/11/15 03:16:52 by mboutuil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include"request.hpp"

void    core_request::read_req(std::string req)
{
    int start;
    int end;
    start = req.find("\r\n");
    _requ_line = req.substr(0,start);
    req = req.substr(start + 1);
    start = req.find("\r\n\r\n");
    _headers = req.substr(0,start);
    req = req.substr(0,start + 3);
    _body = req;
    if (!_body.empty())
        _has_body = true;
    else
        _has_body = false;
}

int main ()
{
     std::string _request = "GET /path/to/resource HTTP/1.1\r\n\
Host: example.com\r\n\
User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/96.0.4664.110 Safari/537.36\r\n\
Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,image/apng,*/*;q=0.8\r\n\
Accept-Encoding: gzip, deflate, br\r\n\
Accept-Language: en-US,en;q=0.5\r\n\
Connection: keep-alive\r\n\r\n\
Optional body content (not standard for GET requests, included here for demonstration)\r\n";
    core_request c_req;
    c_req.read_req(_request);
    request_line reqline;
    reqline.reqline_info(c_req.get_reqline());
    // std::cout << reqline.get_method() << std::endl;
    // std::cout << reqline.get_uri() << std::endl;
    // std::cout << "uri"<< reqline.get_httpversion() << "-"<< std::endl;
}