/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-houd <mel-houd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/13 11:16:08 by mel-houd          #+#    #+#             */
/*   Updated: 2024/11/13 11:51:47 by mel-houd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../Include/Client.hpp"


Client::Client()
{
}


void    Client::SetFd(int fd)
{
    this->fd = fd;
}

int    Client::Recv(int BufferSize)
{
    char    Buffer[BufferSize];
    int Bytes = read(fd, Buffer, BufferSize);
    if (Bytes <= 0)
    {
       return (-1);
    }
    else
    {
        Buffer[Bytes] = '\0';
        std::string	ReqBuffer(Buffer);
        std::cout << ReqBuffer << std::endl;
    }   
    return (0);
}

void    Client::Send()
{
    std::string http_response = "HTTP/1.1 200 OK\r\n"
							"Content-Type: text/plain\r\n"
							"Content-Length: 13\r\n"
							"\r\n"
							"Test\0";
	send(fd, http_response.c_str(), http_response.size(), 0);
}

int     Client::GetFd()
{
    return (fd);
}
