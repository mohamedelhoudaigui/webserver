/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-houd <mel-houd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 11:25:29 by mel-houd          #+#    #+#             */
/*   Updated: 2024/07/21 13:45:34 by mel-houd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Response.hpp"

void    Response::gen_res(Request req)
{
	Generator  gen(req.status, req.status_code, this->server.routes[req.req_line[1]]);
	this->status_line.push_back(HTTP_V);
	this->status_line.push_back(itos(req.status_code));
	this->status_line.push_back(req.status);
	this->headers[C_TYPE] = req.headers[C_TYPE];
	this->headers[C_LEN] = itos(gen.body.length());
	this->body = gen.body;
	for (int i = 0; i < 3; i++)
	{
		this->res.append(this->status_line[i]);
		if (i != 2)
			this->res.append(" ");
	}
	res.append("\r\n");
	for (std::map<std::string, std::string>::iterator it = this->headers.begin(); it != this->headers.end(); ++it)
	{
		this->res.append(it->first);
		this->res.append(it->second);
		this->res.append("\r\n");
	}
	this->res.append("\r\n");
	this->res.append(this->body);
}



Response::Response(Request req, server_config server) : server(server), req(req)
{
	gen_res(req);
}

Response::~Response()
{
	
}