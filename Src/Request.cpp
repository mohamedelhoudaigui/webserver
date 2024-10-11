/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-houd <mel-houd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/11 18:38:25 by mel-houd          #+#    #+#             */
/*   Updated: 2024/10/11 22:25:27 by mel-houd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Include/Request.hpp"

Request::Request(std::string& ReqBuffer): ReqBuffer(ReqBuffer), Status(200)
{}

void	Request::Parse()
{
	std::istringstream	S(ReqBuffer);
	std::string			Line;
	unsigned int		Counter = 0;

	while (getline(S, Line, '\n'))
	{
		if (Status != 200)
			break ;
		if (Counter == 0)
		{
			ParseReqLine(Line);
			Counter++;
		}
		ParseHeaders(Line);
	}
}


void	Request::ParseReqLine(std::string& line)
{
    std::istringstream			S(line);
    std::vector<std::string>	Parts;
    std::string					Part;

    while (S >> Part)
        Parts.push_back(Part);

    if (Parts.size() != 3)
		Status = 400;
	else
	{
		Result.ReqLine.Method = Parts[0];
		Result.ReqLine.Path = Parts[1];
		Result.ReqLine.HttpVersion = Parts[2];	
	}
}

void	Request::ParseHeaders(std::string& Line)
{
	std::istringstream			S(Line);
	std::vector<std::string>	Parts;
	std::string					Part;

	while (getline(S, Part), ':')
        Parts.push_back(Part);

    if (Parts.size() != 2)
		Status = 400;
	else
		Result.Headers[Parts[0]] = Parts[1];
}

// overload :

std::ostream&	operator<<(std::ostream& o, ReqStruct& r)
{
	o << "Request Line :" << std::endl;
	o << r.ReqLine.Method << " "  << r.ReqLine.Path << " " << r.ReqLine.HttpVersion << std::endl;
	o << "Headers :" << std::endl;	
	for (std::map<std::string, std::string>::iterator it = r.Headers.begin(); it != r.Headers.end(); ++it)
	{
		o << it->first << "--" << it->second << std::endl;
	}
	return o;
}