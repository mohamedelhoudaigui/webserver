/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   KqueueObj.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-houd <mel-houd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/13 09:25:41 by mel-houd          #+#    #+#             */
/*   Updated: 2024/12/09 23:39:13 by mel-houd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef KQUEUEOBJ_HPP
#define KQUEUEOBJ_HPP


#include <sys/types.h>
#include <sys/event.h>
#include <sys/time.h>
#include <fcntl.h>
#include <fstream>
#include <vector>
#include <map>
#include <algorithm>
#include <netinet/in.h>
#include <unistd.h>
#include <iostream>


#include "Client.hpp"
#include "Config.hpp"
#include "Types.hpp"

// codes to define event type

#define CLIENT_READ		(void *)0x1
#define CLIENT_WRITE	(void *)0x2
#define SERVER			(void *)0x3
#define TIME			(void *)0x4 
#define CLIENT_TIMEOUT 10 // 10 seconds timeout test


#define MAX_EVENTS 10000 // max (clients + timeout + server) events

class KqueueObj
{
	public:
		KqueueObj(std::fstream& LogFile, std::vector<unsigned int>& ServerSockets, Config& Conf);
		int		Init();
		int		GetKqueueFd();
		void	AddServers();
		void	Run(int (*SetNonBlocking)(int, std::fstream&));

		void	ServerAct(struct kevent& event, int (*SetNonBlocking)(int, std::fstream&));
		void	ClientAct(struct kevent& event);
		void	TimeoutAct(struct kevent& event);

		void	PurgeClient(int fd);

	private:
		int KqueueFd;
		std::fstream&						LogFile;
		struct kevent						event_set;
		struct kevent						events[MAX_EVENTS];
		std::vector<unsigned int>			&ServerSockets;
		std::map<struct kevent, Client>		Clients;
		Config								&Conf;
};

#endif