#include "../../Include/Client.hpp"

Client::Client(struct epoll_event& event, unsigned int port): client_event(event), server_port(port)
{}

