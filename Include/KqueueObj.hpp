/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   KqueueObj.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-houd <mel-houd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/13 09:25:41 by mel-houd          #+#    #+#             */
/*   Updated: 2024/11/14 10:58:00 by mel-houd         ###   ########.fr       */
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
#include <algorithm>
#include <netinet/in.h>
#include <unistd.h>
#include <iostream>


#include "Client.hpp"
#include "Config.hpp"
#include "Types.hpp"

#define CLIENT (void *)0x1
#define SERVER (void *)0x2
#define TIME (void *)0x3
#define CLIENT_TIMEOUT 5 // 5 seconds timeout test


#define MAX_EVENTS 10000

class KqueueObj
{
	public:
		KqueueObj(std::fstream& LogFile, std::vector<unsigned int>& ServerSockets, ConfigFile &Conf);
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
		std::fstream&					LogFile;
		struct kevent					event_set;
		struct kevent					events[MAX_EVENTS];
		std::vector<unsigned int>		&ServerSockets;
		std::map<unsigned int, Client>	Clients;
		ConfigFile						&Conf;
};

#endif