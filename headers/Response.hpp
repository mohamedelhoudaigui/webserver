/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: theworld27 <theworld27@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 11:25:25 by mel-houd          #+#    #+#             */
/*   Updated: 2024/09/02 09:45:03 by theworld27       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RESPONSE_HPP
#define RESPONSE_HPP

#include "Request.hpp"
#include "Utils.hpp"
#include "Generator.hpp"

#include <iostream>
#include <vector>
#include <unistd.h>
#include <cstring>
#include <map>
#include <sstream>
#include <fstream>

#define C_TYPE "Content-Type: "
#define C_LEN "Content-Lenght: "

class	Response
{
	public:
		Response(Request req, server_config server);
		~Response();
		void		gen_res(Request req);
		void		set_MIME_type(Request req);

		std::vector<std::string>			status_line;
		std::map<std::string, std::string>	headers;
		std::string							body;
		std::string							res;
		server_config						server;
		Request								req;
};

#endif