/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-houd <mel-houd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/11 18:38:25 by mel-houd          #+#    #+#             */
/*   Updated: 2024/10/11 21:07:02 by mel-houd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Include/Request.hpp"

Request::Request(std::string& ReqBuffer): ReqBuffer(ReqBuffer), Status(200)
{}

void	Request::Parse()
{
	//std::cout << "9lawi\n";
	std::istringstream	S(ReqBuffer);
	std::string			Line;
	unsigned int		Counter = 0;

	while (getline(S, Line, '\n'))
	{
		if (Counter == 0)
		{
			ParseReqLine(Line);
			std::cout << Result.ReqLine.Method << ' ';
			std::cout << Result.ReqLine.Path << ' ';
			std::cout << Result.ReqLine.HttpVersion << std::endl;
			Counter++;
		}
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