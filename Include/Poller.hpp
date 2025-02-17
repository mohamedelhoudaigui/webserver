#pragma once

#include <vector>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>
#include <algorithm>
#include <stdexcept>

#include "Types.hpp"
#include "Tools.hpp"
#include "HttpRequest.hpp"
#include "Client.hpp"
#include "CGI.hpp"
#include "Config.hpp"

class Poller
{
    public:
        Poller(std::vector<unsigned int>& ServerSockets, Config& config);
        void    Run();
        void    ServerAct(struct epoll_event event);
        void    ClientAct(struct epoll_event event);
        void    ClientPurge(struct epoll_event client);

        int     wrapper_wait();
        void    wrapper_add(int fd, int add_type);
        void    wrapper_delete(int fd);

        ~Poller();

    private:
        int                 epoll_fd;
        struct epoll_event  event; // used to register an event before pushing it to events pool.
        struct epoll_event  events[MAX_EVENTS]; // main events pool.
        std::vector<unsigned int>& ServerSockets;
        std::map<unsigned int, Client> clients;
        Config&                 config;
};

