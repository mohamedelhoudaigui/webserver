/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-houd <mel-houd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 05:20:47 by mel-houd          #+#    #+#             */
/*   Updated: 2024/07/21 10:34:35 by mel-houd         ###   ########.fr       */
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

#define GET			"get"
#define POST		"post"
#define DELETE		"delete"
#define HTTP_V		"HTTP/1.1"
#define HOST		"host"
#define	CONNECTION	"connection"
#define CONT_LEN	"content-length"
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
		void	set_req_prop(std::string status, int status_code);
		void	parse_header(std::string& buffer);
		void	print_req();
		void	check_headers_value();

		std::string							request;
		std::vector<std::string>			req_line;
		std::map<std::string, std::string>	headers;
		std::string							body;
		bool								valid_req;
		std::string							status;
		int									status_code;
		server_config						server;
};

#endif