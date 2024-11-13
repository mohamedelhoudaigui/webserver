/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   SelectObj.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-houd <mel-houd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/13 09:25:41 by mel-houd          #+#    #+#             */
/*   Updated: 2024/11/13 10:15:57 by mel-houd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SELECT_HPP
#define SELECT_HPP


#include <sys/select.h>

enum TYPE {
	READ = 0,
	WRITE = 1,
};

class SelectObj
{
	public :
		SelectObj();
		~SelectObj();
		void	Set(int fd, TYPE t);
		void	Unset(int fd, TYPE t);
		bool	IsReadable(int fd);
		bool	IsWritable(int fd);
		void	SetMaxFd(int fd);
		int		GetMaxFd();
		int		Run();
		

	private:
		fd_set	WriteFds;
		fd_set	ReadFds;
		int		FdsNumber;
	
};


#endif