/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-houd <mel-houd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/11 18:38:40 by mel-houd          #+#    #+#             */
/*   Updated: 2024/10/11 23:07:25 by mel-houd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REQUEST_HPP
#define REQUEST_HPP

#include <iostream>
#include <string>
#include <map>
#include <sstream>
#include <vector>

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
}		ReqStruct;


class	Request
{
	public:
		Request(std::string& ReqBuffer);
		void		Parse();
		void		ParseReqLine(std::string& Line);
		void		ParseHeaders(std::string& Line);
		void		ParseBody(std::string& Line);

		ReqStruct		Result;

	private:
		unsigned int	Status;
		std::string		ReqBuffer;
		
};

std::ostream&	operator<<(std::ostream& o, ReqStruct& r);
std::string		TrimAll(std::string str);

#endif