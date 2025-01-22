/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Poller.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-houd <mel-houd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/22 11:17:27 by mel-houd          #+#    #+#             */
/*   Updated: 2025/01/22 13:04:38 by mel-houd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef POLLER_HPP
#define POLLER_HPP

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

class Poller
{
    public:
        Poller(std::vector<unsigned int>& ServerSockets);
        void    Run();
        void    ServerAct(struct epoll_event event);
        void    ClientAct(struct epoll_event event);
        ~Poller();

    private:
        int                 epoll_fd;
        struct epoll_event  event; // used to register an event before pushing it to events pool.
        struct epoll_event  events[MAX_EVENTS]; // main events pool.
        std::vector<unsigned int>& ServerSockets;
};


#endif
