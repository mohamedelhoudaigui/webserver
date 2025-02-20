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

Poller::Poller(std::vector<unsigned int>& ServerSockets, Config& config): ServerSockets(ServerSockets), config(config)
{
    // Initialize poll fds for server sockets
    for (size_t i = 0; i < ServerSockets.size(); ++i)
    {
        struct pollfd pfd;
        pfd.fd = ServerSockets[i];
        pfd.events = POLLIN;
        pfd.revents = 0;
        fds.push_back(pfd);
    }
}

Poller::~Poller()
{
}

int Poller::wrapper_wait()
{
    int num_events = poll(&fds[0], fds.size(), -1);
    if (num_events == -1)
        throw std::runtime_error("error in poll");
    return num_events;
}

void Poller::wrapper_add(int fd, short events)
{
    struct pollfd pfd;
    pfd.fd = fd;
    pfd.events = events;
    pfd.revents = 0;
    fds.push_back(pfd);
}

void Poller::wrapper_delete(int fd)
{
    std::vector<struct pollfd>::iterator it = fds.begin();
    while (it != fds.end()) {
        if (it->fd == fd) {
            it = fds.erase(it);
        } else {
            ++it;
        }
    }
}

void Poller::ServerAct(struct pollfd& event)
{
    struct sockaddr_in client_addr;
    socklen_t client_len = sizeof(client_addr);

    int client_fd = accept(event.fd, (struct sockaddr*)&client_addr, &client_len);
    if (client_fd == -1)
    {
        Logger(WARNING, "error accepting connection");
        return;
    }

    wrapper_add(client_fd, POLLIN);
}

void Poller::ClientAct(struct pollfd& event)
{
    std::map<int, Client>::iterator it = clients.find(event.fd);
    std::cout << "ClientAct: " << event.fd << std::endl;
    
    if (it == clients.end())
    {
        clients[event.fd] = Client(&config);
        clients[event.fd].SetFd(event.fd);
    }
    
    Client& client = clients[event.fd];
    ssize_t bytes_received = client.Recv(1024);
    
    // Check for connection closure or error
    if (bytes_received <= 0)
    {
        ClientPurge(event);
        return;
    }
    
    // Handle the case where the client is done sending data
    if (event.revents & POLLHUP || event.revents & POLLERR)
    {
        ClientPurge(event);
        return;
    }
}

void Poller::Run()
{
    while (true)
    {
        // Wait for events using poll
		wrapper_wait();
        
        // Process events only if we have any
        for (size_t i = 0; i < fds.size(); ++i)
        {
            if (fds[i].revents == 0)
                continue;

            std::vector<unsigned int>::iterator it = ServerSockets.begin();
            bool isServerSocket = false;
            
            for (; it != ServerSockets.end(); ++it) {
                if (*it == static_cast<unsigned int>(fds[i].fd)) {
                    isServerSocket = true;
                    break;
                }
            }

            if (isServerSocket)
                ServerAct(fds[i]);
            else
                ClientAct(fds[i]);

            fds[i].revents = 0;
        }
    }
}

void Poller::ClientPurge(struct pollfd& event)
{
    clients.erase(event.fd);
    wrapper_delete(event.fd);
    close(event.fd);
    Logger(INFO, "client purged");
}
