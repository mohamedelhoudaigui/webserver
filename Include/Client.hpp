/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-houd <mel-houd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/13 11:16:20 by mel-houd          #+#    #+#             */
/*   Updated: 2024/12/09 23:44:36 by mel-houd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
#define CLIENT_HPP

#include "HttpRequest.hpp"
#include <string>
#include <sys/socket.h>
#include <iostream>
#include <unistd.h>
#include <fstream>
#include <sstream>


class   Client
{
	private:
		int             fd;
		std::string     Request;
		std::string     Response;
		std::fstream&   LogFile;
		HttpRequestParser *parser;

	public:
		Client(std::fstream& LogFile);
		int			Recv(int BufferSize);
		void		Send();
		void		SetFd(int fd);
		int			GetFd();
		int 		handleRequest(const std::string& requestData);
		Client& operator=(const Client& other) const;
		~Client();
};


#endif