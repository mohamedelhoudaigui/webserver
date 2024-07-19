/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-houd <mel-houd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 05:20:47 by mel-houd          #+#    #+#             */
/*   Updated: 2024/07/19 08:24:49 by mel-houd         ###   ########.fr       */
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
#include <stdbool.h>

#include "../utils/utils.hpp"
#include "../structes.hpp"

#define GET "GET"
#define POST "POST"
#define DELETE "DELETE"
#define HTTP_V "HTTP/1.1"
#define BAD_REQ		400
#define NOT_FOUND	404
#define	HTTP_V_NSUP	505


class	Request
{
	public:
		Request(char *req, server_config server);
		~Request();

		void	parse_req();
		void	parse_req_line();
		void	parse_header(std::string& buffer);
		void	print_req();

		std::string							request;
		std::vector<std::string>			req_line;
		std::map<std::string, std::string>	headers;
		std::string							body;
		bool								valid_req;
		std::string							err;
		int									err_code;
		server_config						server;

};

std::string 				trim(const std::string &s);
std::vector<std::string>	split(const std::string &buffer, char delimiter);
void						string_lower(const std::string& input);
int							find_item(std::vector<std::string>& vector, std::string& needle);

#endif