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


Client::Client(std::fstream& LogFile): LogFile(LogFile)
{}

void    Client::SetFd(int fd)
{
    this->fd = fd;
}

int    Client::Recv(int BufferSize)
{
   char	Buffer[BufferSize];
		
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
