/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-houd <mel-houd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/13 11:16:08 by mel-houd          #+#    #+#             */
/*   Updated: 2024/11/17 11:20:43 by mel-houd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../Include/Client.hpp"
#include "../../Include/HttpRequest.hpp"

Client::Client(std::fstream& LogFile): LogFile(LogFile)
{}

void    Client::SetFd(int fd)
{
    this->fd = fd;
}

int    Client::Recv(int BufferSize)
{
    char	Buffer[BufferSize];
	HttpRequest req;
    int		count = recv(fd, Buffer, BufferSize, 0);
    if (count < 0)
    {
        this->LogFile << "Error in reading" << std::endl;
        return (-1);
    }
    else if (count == 0)
    {
        std::cout << "Client disconnected" << std::endl;
        return (0);
    }

    Buffer[count] = '\0';
    std::cout << Buffer << std::endl;
    this->Request = Buffer;
    //TODO: Request Parser here
    size_t reqline_end = Request.find("\r\n");
    if (reqline_end != std::string::npos) {
        std::string reqline = Request.substr(0, reqline_end);
        if (!req.Parser(reqline)) {
            LogFile << "Invalid Request" << std::endl;
            // Response here should throw 400 error
            Send();
            return -1;
        }
        std::string remainingRequest = Request.substr(reqline_end + 2);
        std::istringstream req_stream(remainingRequest);
        std::string headerline;
        bool    host = false;

        while(std::getline(req_stream, headerline)) {
            if (!headerline.empty() && headerline[headerline.length() - 1] == '\r')
                headerline.erase(headerline.length() - 1);
            //mark end of headers
            if (headerline.empty())
                break;
            //validate header format
            if (!HttpHeaders::isValidHeader(headerline)) {
                LogFile << "Invalid header" << headerline << std::endl;
                return -1;
            }
            //check host
            if (headerline.substr(0, 5) == "Host:") {
                host = true;
                std::string host_value = headerline.substr(6);
                HttpHeaders::trim(host_value, " \t");
                if (!HttpHeaders::isValidHost(host_value)) {
                    LogFile << "Invalid Host value" << std::endl;
                    // need to send 400 bad request here
                    //Send should be called
                    return -1;
                }
                if (headerline.substr(0, 14) == "Content-Length:") {
                    std::string value = headerline.substr(15);
                    HttpHeaders::trim(value, " \t");
                    if (!HttpHeaders::isValidContentLength(value)) {
                        LogFile << "Invalid Content-length value" << std::endl;
                        // send 400 Bad Request
                        return -1;
                    }
                    size_t content_length = static_cast<size_t>(std::atol(value.c_str()));
                    // if (content_length > Conf)
                }
            }
        }
    }
    return (1);
}

void    Client::Send()
{
    this->Response.append("HTTP/1.1 200 OK\r\n");
    this->Response.append("Content-Type: text/plain\r\n");
    this->Response.append("Content-Length: 0\r\n");
    this->Response.append("\r\n");
    this->Response.append("");
	send(fd, Response.c_str(), Response.size(), 0);
}

int     Client::GetFd()
{
    return (fd);
}
