/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   SelectObj.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-houd <mel-houd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/13 09:25:44 by mel-houd          #+#    #+#             */
/*   Updated: 2024/11/13 12:48:02 by mel-houd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../Include/SelectObj.hpp"
#include <cstddef>

SelectObj::SelectObj()
{
	FD_ZERO(&WriteFds);
	FD_ZERO(&ReadFds);
	FdsNumber = 2;
}

void    SelectObj::Set(int fd, TYPE t)
{
	switch (t)
	{
		case READ:
			FD_SET(fd, &ReadFds);
			break;
	
		case WRITE:
			FD_SET(fd, &WriteFds);
			break;
	}
}

void	SelectObj::Unset(int fd, TYPE t)
{
	switch (t)
	{
		case READ:
			FD_CLR(fd, &ReadFds);
			break;

		case WRITE:
			FD_CLR(fd, &WriteFds);
			break;
	}
}

bool	SelectObj::IsReadable(int fd)
{
	if (FD_ISSET(fd, &ReadFds) != 0)
		return (true);
	return (false);
}

bool	SelectObj::IsWritable(int fd)
{
	if (FD_ISSET(fd, &WriteFds) != 0)
		return (true);
	return (false);
}

void		SelectObj::SetMaxFd(int fd)
{
	FdsNumber = fd;
}


int		SelectObj::GetMaxFd()
{
	return (FdsNumber);
}

int	SelectObj::Run()
{
	return (select(FdsNumber + 1, &ReadFds, &WriteFds, NULL, NULL));
}

SelectObj::~SelectObj()
{
	
}
