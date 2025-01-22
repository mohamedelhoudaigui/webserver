#include "../../Include/Client.hpp"
#include "../../Include/HttpRequest.hpp"

Client::Client()
{
    parser = new HttpRequestParser(8192);
}

void    Client::SetFd(int fd)
{
    this->fd = fd;
}


int Client::handleRequest(const std::string& requestData) {
    
    if (!parser->feed(requestData))
    {
        Response = "HTTP/1.1 400 Bad Request\r\n\r\n";
        Send();
        return (-1);
    }

    if (parser->isComplete())
    {
        HttpRequest* request = parser->getRequest();
        if (!request) {
            Response = "HTTP/1.1 400 Bad Request\r\n\r\n";
            Send();
            return -1;
        }

        Response = "HTTP/1.1 200 OK\r\n";
        Response += "Content-Type: text/plain\r\n";
        Response += "Content-Length: 0\r\n\r\n";

        delete request;
        parser->reset();
        return 1;
    }
    return 1;
}

int    Client::Recv(int BufferSize)
{
    char	Buffer[BufferSize];
    int		count = recv(fd, Buffer, BufferSize, 0);
    if (count < 0)
        return (-1);
    else if (count == 0)
    {
        std::cout << "Client disconnected" << std::endl;
        return (0);
    }

    Buffer[count] = '\0';
    Request.append(Buffer);
    std::cout << Buffer << std::endl; 
    return (handleRequest(Buffer));
}

void    Client::Send()
{
    // this->Response.append("HTTP/1.1 200 OK\r\n");
    // this->Response.append("Content-Type: text/plain\r\n");
    // this->Response.append("Content-Length: 0\r\n");
    // this->Response.append("\r\n");
    // this->Response.append("");
	send(fd, Response.c_str(), Response.size(), 0);
}

int     Client::GetFd()
{
    return (fd);
}

Client::~Client()
{
    // delete parser;
}