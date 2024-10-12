/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-houd <mel-houd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/12 02:06:10 by mel-houd          #+#    #+#             */
/*   Updated: 2024/10/12 02:25:46 by mel-houd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RESPONSE_HPP
#define RESPONSE_HPP

#include <iostream>
#include <string>
#include <map>
#include <sstream>
#include <vector>

#include "Request.hpp"

typedef	struct StatusLine
{
	std::string	HttpVersion;
	std::string	StatusCode;
	std::string	StatusText;
}	StatusLine;

typedef struct ResStruct
{
	StatusLine							Stline;
	std::map<std::string, std::string>	Headers;
	std::vector<std::string>			Body;
	std::string							BufferRes;
	char								*RawResponse;
}				ResStruct;


class	Response
{
	public:
		Response(Request req);
		void		BuildSLine();
		void		AppendHeaders();
		void		AppendBody();
		ResStruct	GetRes();
		
	private:
		ResStruct	res;
};



#endif