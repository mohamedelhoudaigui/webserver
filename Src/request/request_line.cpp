/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   request_line.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mboutuil <mboutuil@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 02:43:35 by mboutuil          #+#    #+#             */
/*   Updated: 2024/11/15 03:11:50 by mboutuil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include"request.hpp"

void    request_line::reqline_info(std::string _reqline)
{
    int start;
    std::string buff;
    start = _reqline.find_first_not_of(" \t\r\n");
    _reqline = _reqline.substr(start);
    start = _reqline.find_first_of(" ");
    if (_reqline.at(start + 1) == ' ' || _reqline.at(start + 1) == '\t')
    {
        std::cout << "error";
        // throw BAD_REQUEST;
        return ;
    }
    buff = _reqline.substr(0,start);
    // std::cout << "-"<<buff <<"-"<< std::endl;
    if (buff == "GET")
        method = GET;
    else if (buff == "POST")
        method = POST;
    else if (buff == "DELETE")
        method = DELETE;
    else
    {
        std::cout << "error method";
        // throw BAD_REQUEST
        return ;
    }
    _reqline = _reqline.substr(start + 1);
    start = _reqline.find_first_of(" ");
    if (_reqline.at(start + 1) == ' ' || _reqline.at(start + 1) == '\t')
    {
        std::cout << "error uri";
        // throw BAD_REQUEST;
        return ;
    }
    uri = _reqline.substr(0,start);
    _reqline = _reqline.substr(start + 1);
    start = _reqline.find_last_not_of(" \t\r\n");
    http_version = _reqline.substr(0,start);
    if (http_version.empty() || uri.empty())
    {
        std::cout << "error httpversion";
        // throw BAD_REQUEST;
        return ;

    }
    }

enum _methods request_line::get_method()
{
    return this->method;
}

std::string request_line::get_uri()
{
    return this->uri;
}
std::string request_line::get_httpversion()
{
    return this->http_version;
}