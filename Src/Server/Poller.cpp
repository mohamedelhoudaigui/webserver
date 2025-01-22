/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Poller.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-houd <mel-houd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/22 11:17:32 by mel-houd          #+#    #+#             */
/*   Updated: 2025/01/22 13:04:27 by mel-houd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../Include/Poller.hpp"



Poller::Poller(std::vector<unsigned int>& ServerSockets): ServerSockets(ServerSockets)
{
    epoll_fd = epoll_create1(0);
    if (epoll_fd == -1)
    {
        close_sockets(ServerSockets);
        throw std::runtime_error("error creating epoll instance");
    }

    for (int i = 0; i < ServerSockets.size(); ++i)
    {
        event.events = EPOLLIN;
        event.data.fd = ServerSockets[i];
        if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, ServerSockets[i], &event) == -1)
        {
            Logger(WARNING, "error adding server socket to epoll");
            close(ServerSockets[i]);
        }
        else
            Logger(DEBUG, "server socket added to epoll");
    }
}

Poller::~Poller()
{
    close(epoll_fd);
}


void    Poller::Run()
{
    while (true)
    {
        int num_events = epoll_wait(epoll_fd, events, MAX_EVENTS, -1);
        if (num_events == -1)
            throw std::runtime_error("error in epoll_wait");

        for (int i = 0; i < num_events; ++i)
        {
            if (find(ServerSockets.begin(), ServerSockets.end(), events[i].data.fd) != ServerSockets.end())
            {
                struct sockaddr_in client_addr;
                socklen_t client_len = sizeof(client_addr);

                int client_fd = accept(events[i].data.fd, (struct sockaddr*)&client_addr, &client_len);
                if (client_fd == -1)
                {
                    Logger(WARNING, "error accepting connection");
                    continue;
                }

                event.events = EPOLLIN;
                event.data.fd = client_fd;

                if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, client_fd, &event) == -1)
                {
                    Logger (WARNING, "error adding client socket to epoll");
                    close(client_fd);
                }
            }
            else
            {
                char buffer[1024];
                int bytes_received = recv(events[i].data.fd, buffer, sizeof(buffer), 0);

                if (bytes_received <= 0)
                {
                    Logger(INFO, "client disconnected");
                    close(events[i].data.fd);
                }
                else
                {
                    buffer[bytes_received] = '\0';
                    Logger (DEBUG, std::string("Received: ") + buffer);
                }
            }
        }
    }
}