#pragma once

#include <vector>
#include <poll.h>
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
        void    ServerAct(struct pollfd& event);
        void    ClientAct(struct pollfd& event);
        void    ClientPurge(struct pollfd& event);

        int     wrapper_wait();
        void    wrapper_add(int fd, short events);
        void    wrapper_delete(int fd);

        ~Poller();

    private:
        std::vector<struct pollfd> fds;
        std::vector<unsigned int>& ServerSockets;
		std::map<int, Client> clients;
        Config& config;
};

