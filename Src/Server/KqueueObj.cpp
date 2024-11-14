/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   KqueueObj.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-houd <mel-houd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/13 09:25:44 by mel-houd          #+#    #+#             */
/*   Updated: 2024/11/14 04:13:45 by mel-houd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../Include/KqueueObj.hpp"

KqueueObj::KqueueObj(std::fstream& LogFile, std::vector<unsigned int>& ServerSockets): LogFile(LogFile), ServerSockets(ServerSockets)
{
}



int	KqueueObj::Init()
{
	int kq;

	if ((kq = kqueue()) == -1)
	{
		this->LogFile << "Error initing in kqueue" << std::endl;
		return (-1);
	}
	this->KqueueFd = kq;
	return (0);
}

int	KqueueObj::GetKqueueFd()
{
	return (KqueueFd);
}

void	KqueueObj::AddServers()
{
	for (int i = 0; i < ServerSockets.size(); ++i)
	{
		EV_SET(&event_set, ServerSockets[i], EVFILT_READ, EV_ADD, 0, 0, (void *)1);
		if (kevent(KqueueFd, &event_set, 1, NULL, 0, NULL) == -1)
		{
			this->LogFile << "Error in kevent" << std::endl;
		}
	}
}

void	KqueueObj::Run(int (*SetNonBlocking)(int, std::fstream&))
{
	while (1)
	{
		int number_events = kevent(KqueueFd, NULL, 0, events, MAX_EVENTS, NULL);
		if (number_events == -1)
			this->LogFile << "Error in kevent (while loop)" << std::endl;

		for (int i = 0; i < number_events; ++i) // loop on how much events we have
		{
			if (events[i].flags & EV_ERROR) // check if event flag is set to error
				this->LogFile << "EV_ERROR is set in event number " << i << std::endl;

			ServerAct(events[i], SetNonBlocking);
			ClientAct(events[i]);
		}
	}
}

void	KqueueObj::ServerAct(struct kevent& event, int (*SetNonBlocking)(int, std::fstream&))
{
	struct sockaddr_in					ClientAddr;
	socklen_t							ClientLen = sizeof(ClientAddr);

	if (event.udata == (void *)1)
	{
		// Handle new connection
		int client_fd = accept(event.ident, (struct sockaddr *)&ClientAddr, &ClientLen);
		if (client_fd == -1) {
			this->LogFile << "Error in accpet server socket number " << event.ident << std::endl;
		}
		// Set new socket to non-blocking
		if (SetNonBlocking(client_fd, LogFile) == -1)
			close(client_fd);
		
		// Add new client to kqueue
		EV_SET(&event_set, client_fd, EVFILT_READ, EV_ADD, 0, 0, NULL);
		if (kevent(KqueueFd, &event_set, 1, NULL, 0, NULL) == -1)
		{
			this->LogFile << "Failed to add new client to queue" << std::endl;
			close(client_fd);
		}
		std::cout << "New client connected" << std::endl;
	}
}


void	KqueueObj::ClientAct(struct kevent& event)
{
	if (event.udata == NULL)
	{
		char	Buffer[1024];
		
		int		count = read(event.ident, Buffer, 1024);
		if (count == -1)
		{
			this->LogFile << "Error in reading" << std::endl;
		}
		else if (count == 0)
		{
			std::cout << "Client disconnected" << std::endl;
			close(event.ident);
		}
		else
		{
			Buffer[count] = '\0';
			std::cout << Buffer << std::endl;
			write(event.ident, Buffer, count);
			close(event.ident);
		}
	}
}