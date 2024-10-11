/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-houd <mel-houd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/11 18:38:25 by mel-houd          #+#    #+#             */
/*   Updated: 2024/10/12 00:54:58 by mel-houd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Include/Request.hpp"

Request::Request(std::string& ReqBuffer): ReqBuffer(ReqBuffer), Status(200)
{}

std::string	TrimAll(std::string str)
{
    size_t start = 0;
    while (start < str.size() && (std::isspace(str[start]) || str[start] == '\n' || str[start] == '\r'))
        ++start;

    if (start == str.size())
        return ("");

    size_t end = str.size() - 1;
    while (end > start && (std::isspace(str[end]) || str[end] == '\n' || str[end] == '\r'))
        --end;

    return (str.substr(start, end - start + 1));
}

void	Request::Parse()
{
	std::istringstream	S(ReqBuffer);
	std::string			Line;
	unsigned int		Counter = 0;

	while (getline(S, Line, '\n'))
	{
		Line = TrimAll(Line);
		if (Status != 200)
			break ;
		if (Counter == 0)
		{
			ParseReqLine(Line);
			Counter++;
		}
		else if (Line != "")
			ParseHeaders(Line);
		else
			ParseBody(Line);
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
	{
		Status = 400;
	}
	else
	{
		Result.ReqLine.Method = TrimAll(Parts[0]);
		Result.ReqLine.Path = TrimAll(Parts[1]);
		Result.ReqLine.HttpVersion = TrimAll(Parts[2]);	
	}
}

void	Request::ParseHeaders(std::string& Line)
{
	size_t pos = Line.find(':');
    if (pos == std::string::npos) // carage return 
	{
		Status = 400;
	}
	else
		Result.Headers[TrimAll(Line.substr(0, pos))] = TrimAll(Line.substr(pos + 1));
}

void	Request::ParseBody(std::string& Line)
{
	this->Result.Body += Line;
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
	o << "Body:" << std::endl;
	o << r.Body;
	return o;
}
