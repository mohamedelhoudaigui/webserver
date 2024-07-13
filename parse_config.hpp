/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_config.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-houd <mel-houd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/11 22:01:42 by mel-houd          #+#    #+#             */
/*   Updated: 2024/07/12 23:51:45 by mel-houd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSE_CONFIG_HPP
#define PARSE_CONFIG_HPP

#include <iostream>
#include <vector>
#include <map>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <cstring>
#include <arpa/inet.h>
#include <fcntl.h>
#include <poll.h>
#include <sstream>
#include <fstream>
#include <algorithm>

#define MAX_PORT 65535
#define MIN_PORT 0
#define MIN_STATUS 100
#define MAX_STATUS 599
#define MAX_BODY_SIZE 10000000
#define GET "GET"
#define POST "POST"
#define DELETE "DELETE"
#define TAB '\t'
#define SPACE ' '

typedef	struct
{
	std::string							host;
	int									port;
	std::map<int, std::string>			error_pages;
	std::map<std::string, std::vector<std::string> >	routes; // first param is vector is path other are allowed methods
	unsigned int						body_size;
}		server_config;			

class	Parse_config
{
	public:
		std::vector<server_config>	servers;

		Parse_config(std::string file_name);
		~Parse_config();

		void	parse();

	private:
		std::ifstream				file_strm;
};

#endif