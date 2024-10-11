/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-houd <mel-houd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/11 18:38:40 by mel-houd          #+#    #+#             */
/*   Updated: 2024/10/11 21:07:10 by mel-houd         ###   ########.fr       */
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
		void	Parse();
		void	ParseReqLine(std::string& line);
		void	ParseHeaders();
		void	ParseBody();

	private:
		unsigned int	Status;
		std::string		ReqBuffer;
		ReqStruct		Result;
		
};

#endif