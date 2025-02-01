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
    }
}

Poller::~Poller()
{
    close(epoll_fd);
}

int Poller::wrapper_wait()
{
    int num_events = epoll_wait(epoll_fd, events, MAX_EVENTS, -1);
    if (num_events == -1)
        throw std::runtime_error("error in epoll_wait");
    return (num_events);
}

void    Poller::wrapper_add(int fd, int add_type)
{
    event.events = add_type;
    event.data.fd = fd;

    if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, fd, &event) == -1)
    {
        Logger (WARNING, "error adding client socket to epoll");
        close(fd);
    }
}

void    Poller::wrapper_delete(int fd)
{
    if (epoll_ctl(epoll_fd, EPOLL_CTL_DEL, fd, NULL) == -1)
    {
        Logger (WARNING, "error removing client socket from epoll");
        close(fd);
    }
}

void    Poller::ServerAct(struct epoll_event event)
{
    struct sockaddr_in client_addr;
    socklen_t client_len = sizeof(client_addr);

    int client_fd = accept(event.data.fd, (struct sockaddr*)&client_addr, &client_len);
    if (client_fd == -1)
    {
        Logger(WARNING, "error accepting connection");
        return ;
    }

    wrapper_add(client_fd, EPOLLIN);
}

void Poller::ClientAct(struct epoll_event event)
{

    if (clients.find(event.data.fd) == clients.end())
    {
        clients[event.data.fd] = Client();
        clients[event.data.fd].SetFd(event.data.fd);
    }
    Client& client = clients[event.data.fd];
    client.Recv(1024);
    /*int ret = req.ReadBuffer(event); // read and parse 1 recv buffer*/
    /*if (ret == -1) // recv consumed all the request*/
    /*    ClientPurge(event);*/
    /*else*/
    /*{*/
    /*    switch (req.status)*/
    /*    {*/
    /*        case INIT:*/
    /*            std::cout << "INIT" << std::endl;*/
    /*            break ;*/
    /*        case OK:*/
    /*            std::cout << "OK" << std::endl;*/
    /*            break ;*/
    /*        case PENDING:*/
    /*            std::cout << "PENDING" << std::endl;*/
    /*            break ;*/
    /*        case INVALID:*/
    /*            std::cout << "INVALID" << std::endl;*/
    /*            break ;*/
    /*    }*/
    /*}*/
    //print_info(req.GetResult());
}

void    Poller::Run()
{
    while (true)
    {
        int num_events = wrapper_wait();

        for (int i = 0; i < num_events; ++i)
        {
            if (find(ServerSockets.begin(), ServerSockets.end(), events[i].data.fd) != ServerSockets.end())
                ServerAct(events[i]);
            else
                ClientAct(events[i]);
        }
    }
}

void    Poller::ClientPurge(struct epoll_event client)
{
    clients.erase(client.data.fd);
    wrapper_delete(client.data.fd);
    close(client.data.fd);
    Logger(INFO, "client purged");
}
