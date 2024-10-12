/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-houd <mel-houd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/11 18:38:25 by mel-houd          #+#    #+#             */
/*   Updated: 2024/10/12 02:24:02 by mel-houd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Include/Request.hpp"

Request::Request(std::string& ReqBuffer): ReqBuffer(ReqBuffer)
{
	Result.Status = 200;
}

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
	bool				h = true;

	getline(S, Line, '\n');
	Line = TrimAll(Line);
	ParseReqLine(Line);
	while (getline(S, Line, '\n') && Result.Status == 200)
	{
		Line = TrimAll(Line);
		if (h == true)
			ParseHeaders(Line, h);
		else
			ParseBody(Line);
	}
}

ReqStruct	Request::GetResult()
{
	return (this->Result);
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
		Result.Status = 400;
	}
	else
	{
		Result.ReqLine.Method = TrimAll(Parts[0]);
		Result.ReqLine.Path = TrimAll(Parts[1]);
		Result.ReqLine.HttpVersion = TrimAll(Parts[2]);	
	}
}

void	Request::ParseHeaders(std::string& Line, bool& h)
{
	size_t pos = Line.find(':');
	if (Line.empty())
		h = false;
    else if (pos == std::string::npos)
		Result.Status = 400;
	else
		Result.Headers[TrimAll(Line.substr(0, pos))] = TrimAll(Line.substr(pos + 1));
}

void	Request::ParseBody(std::string& Line)
{
	this->Result.Body.push_back(Line);
}

// overload :

std::ostream&	operator<<(std::ostream& o, Request& req)
{
	ReqStruct	r = req.GetResult();
	if (r.Status != 200)
	{
		o << "Error : Status " << r.Status << std::endl;
		return o;
	}
	o << "Request Line :" << std::endl;
	o << r.ReqLine.Method << " "  << r.ReqLine.Path << " " << r.ReqLine.HttpVersion << std::endl;
	o << "Headers :" << std::endl;	
	for (std::map<std::string, std::string>::iterator it = r.Headers.begin(); it != r.Headers.end(); ++it)
	{
		o << it->first << "--" << it->second << std::endl;
	}
	o << "Body:" << std::endl;
	for (std::vector<std::string>::iterator it = r.Body.begin(); it != r.Body.end(); ++it)
	{
		o << *it << std::endl;
	}
	return o;
}
