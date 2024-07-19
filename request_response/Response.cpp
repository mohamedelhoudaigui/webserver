/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-houd <mel-houd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 11:25:29 by mel-houd          #+#    #+#             */
/*   Updated: 2024/07/19 09:34:38 by mel-houd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Response.hpp"


void    Response::gen_res(std::string& status, int status_code)
{   
	Generator  gen(status, status_code);
	this->status_line.push_back(HTTP_V);
	this->status_line.push_back(itos(status_code));
	this->status_line.push_back(status);
	this->headers[C_TYPE] = TEXT_CTYPE;
	this->headers[C_LEN] = gen.body.size();
	this->body = gen.body;
	for (int i = 0; i < 3; i++)
	{
		this->res.append(this->status_line[i]);
		if (i != 2)
			this->res.append(" ");
		else
			this->res.append("\r\n");
	}
	for (std::map<std::string, std::string>::iterator it = this->headers.begin(); it != this->headers.end(); ++it)
	{
		this->res.append(it->first);
		this->res.append(it->second);
		this->res.append("\r\n");
	}
	this->res.append("\r\n");
	this->res.append(this->body);
}


Response::Response(Request req)
{
	gen_res(req.err, req.err_code);
}

Response::~Response()
{
	
}