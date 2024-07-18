/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-houd <mel-houd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 11:25:25 by mel-houd          #+#    #+#             */
/*   Updated: 2024/07/18 23:20:55 by mel-houd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RESPONSE_HPP
#define RESPONSE_HPP

#include "Request.hpp"

#include <iostream>
#include <vector>
#include <unistd.h>
#include <cstring>
#include <map>
#include <sstream>
#include <fstream>

class	Response
{
	public:
		Response(Request req);
		~Response();
		std::string	getCurrentDate();
		std::vector<std::string>			status_line;
		std::map<std::string, std::string>	headers;
		std::string							body;
		const char							*res_buffer;
		std::string							res;
};

#endif