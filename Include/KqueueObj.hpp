/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   KqueueObj.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-houd <mel-houd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/13 09:25:41 by mel-houd          #+#    #+#             */
/*   Updated: 2024/11/14 04:05:06 by mel-houd         ###   ########.fr       */
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


#define MAX_EVENTS 100

class KqueueObj
{
	public:
		KqueueObj(std::fstream& LogFile, std::vector<unsigned int>& ServerSockets);
		int		Init();
		int		GetKqueueFd();
		void	AddServers();
		void	Run(int (*SetNonBlocking)(int, std::fstream&));
		void	ServerAct(struct kevent& event, int (*SetNonBlocking)(int, std::fstream&));
		void	ClientAct(struct kevent& event);

	private:
		int KqueueFd;
		std::fstream&				LogFile;
		struct kevent				event_set;
		struct kevent				events[MAX_EVENTS];
		std::vector<unsigned int>	&ServerSockets;
};

#endif