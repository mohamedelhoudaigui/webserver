/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-houd <mel-houd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 11:25:25 by mel-houd          #+#    #+#             */
/*   Updated: 2024/07/21 10:41:13 by mel-houd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RESPONSE_HPP
#define RESPONSE_HPP

#include "Request.hpp"
#include "../utils/utils.hpp"
#include "../gen_template/Generator.hpp"

#include <iostream>
#include <vector>
#include <unistd.h>
#include <cstring>
#include <map>
#include <sstream>
#include <fstream>

#define TEXT_CTYPE "text/html; charset=UTF-8"
#define C_TYPE "Content-Type: "
#define C_LEN "Content-Lenght: "

class	Response
{
	public:
		Response(Request req, server_config server);
		~Response();
		void		gen_res(Request req);

		std::vector<std::string>			status_line;
		std::map<std::string, std::string>	headers;
		std::string							body;
		std::string							res;
		server_config						server;
		Request								req;
};

#endif