/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-houd <mel-houd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/11 18:38:40 by mel-houd          #+#    #+#             */
/*   Updated: 2024/10/13 05:32:56 by mel-houd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REQUEST_HPP
#define REQUEST_HPP

#include <iostream>
#include <string>
#include <map>
#include <sstream>
#include <vector>

#include "Helper.hpp"

typedef struct RequestLine
{
	std::string	Method;
	std::string	Path;
	std::string	HttpVersion;
}		RequestLine;

typedef struct ReqStruct
{
	RequestLine							ReqLine;
	std::map<std::string, std::string>	Headers;
	std::vector<std::string>			Body;
	unsigned int						Status;
}		ReqStruct;


class	Request
{
	public:
		Request(std::string& ReqBuffer);
		void		Parse();
		void		ParseReqLine(std::string& Line);
		void		ParseHeaders(std::string& Line, bool& h);
		void		ParseBody(std::string& Line);
		ReqStruct	GetResult();


	private:
		std::string		ReqBuffer;
		ReqStruct		Result;

};

std::ostream&	operator<<(std::ostream& o, Request& r);

#endif