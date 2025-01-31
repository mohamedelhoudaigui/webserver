#ifndef CLIENT_HPP
#define CLIENT_HPP

#include "Request.hpp"
#include "Response.hpp"
#include "Types.hpp"

class Client
{
    public:
        Client(struct epoll_event& event, unsigned int port);

    private:
        struct          epoll_event& client_event;
        unsigned int    server_port;
        Request         client_request;
        Response         client_response;

};

#endif
