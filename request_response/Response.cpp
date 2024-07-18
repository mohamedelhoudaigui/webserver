/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-houd <mel-houd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 11:25:29 by mel-houd          #+#    #+#             */
/*   Updated: 2024/07/18 23:20:58 by mel-houd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Response.hpp"

std::string Response::getCurrentDate() {
    time_t now = time(0);
    struct tm tm_struct;
    char buf[80];

    gmtime_r(&now, &tm_struct);
    strftime(buf, sizeof(buf), "%a, %d %b %Y %H:%M:%S GMT", &tm_struct);

    return std::string(buf);
}

Response::Response(Request req)
{
    std::ifstream  file("./www/index.html");
    std::stringstream fileContents;

     if (!file)
    {
        std::cerr << "Could not open the file!" << std::endl;
        exit(1);
    }
    fileContents << file.rdbuf();
    // Set the status line
    this->status_line.push_back("HTTP/1.1");
    this->status_line.push_back("200");
    this->status_line.push_back("OK\r\n");

    // Set the headers
    this->headers["Date: "] = getCurrentDate();
    this->headers["Date: "].append("\r\n");
    this->headers["Server: "] = "webserve /1.0 (unix)\r\n";
    
    this->headers["Content-Type: "] = "text/html; charset=UTF-8\r\n";
    this->headers["Connection: "] = "close\r\n";

    // Set the body
    this->body = fileContents.str();
    file.close();
    this->headers["Content-Length: "] = std::to_string(this->body.length());
    this->headers["Content-Length: "].append("\r\n");;
    
    for (int i = 0; i < this->status_line.size(); i++)
    {
        res.append(this->status_line[i]);
        if (i != this->status_line.size() - 1)
            res.append(" ");
    }
    for (auto& pair : this->headers)
    {
        std::string tmp;
        tmp.append(pair.first);
        tmp.append(pair.second);
        res.append(tmp);
    }
    res.append("\r\n");
    res.append(this->body);
    this->res_buffer = res.c_str();
}

Response::~Response()
{
    
}