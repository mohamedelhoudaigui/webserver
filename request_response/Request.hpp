/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-houd <mel-houd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 05:20:47 by mel-houd          #+#    #+#             */
/*   Updated: 2024/07/16 09:52:55 by mel-houd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REQUEST_HPP
#define REQUEST_HPP

#include <iostream>
#include <vector>
#include <unistd.h>
#include <cstring>
#include <map>
#include <sstream>


class	Request
{
	public:
		Request(char *req);
		~Request();

		std::string	request;
		void	print_req();
		std::string method;
		std::string url;
		std::string http_version;
		std::map<std::string, std::string> headers;
		std::string body;

};

std::string trim(const std::string &s);
std::vector<std::string> split(const std::string &buffer, char delimiter);

#endif