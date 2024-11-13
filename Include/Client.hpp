/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-houd <mel-houd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/13 11:16:20 by mel-houd          #+#    #+#             */
/*   Updated: 2024/11/13 11:48:41 by mel-houd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <string>
#include <sys/socket.h>
#include <iostream>
#include <unistd.h>


class   Client
{
    public:
        Client();
        int     Recv(int BufferSize);
        void    Send(); // testing
        void    SetFd(int fd);
        int     GetFd();

    private:
        int fd;
};


#endif