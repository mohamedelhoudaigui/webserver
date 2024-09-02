/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: theworld27 <theworld27@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 11:25:29 by mel-houd          #+#    #+#             */
/*   Updated: 2024/09/02 10:58:51 by theworld27       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/Response.hpp"

void    Response::gen_res(Request req)
{
	Generator  gen(req.status, req.status_code, this->server.routes[req.req_line[1]]);
	this->status_line.push_back(HTTP_V);
	this->status_line.push_back(itos(req.status_code));
	this->status_line.push_back(req.status);
	this->headers[C_LEN] = itos(gen.body.length());
	this->body = gen.body;
	set_MIME_type(req);
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


void	Response::set_MIME_type(Request req)
{
	std::map<std::string, std::string> mime_types;
	std::map<std::string, std::string>::iterator it;

	mime_types[".html"] = "text/html";
    mime_types[".htm"] = "text/html";
    mime_types[".css"] = "text/css";
    mime_types[".js"] = "application/javascript";
    mime_types[".png"] = "image/png";
    mime_types[".jpg"] = "image/jpeg";
    mime_types[".jpeg"] = "image/jpeg";
    mime_types[".gif"] = "image/gif";
    mime_types[".json"] = "application/json";
    mime_types[".xml"] = "application/xml";
	mime_types[".mp3"] = "audio/mpeg";
	
	std::string file = this->server.routes[req.req_line[1]];
	size_t pos = file.find_last_of(".");
	if (pos != std::string::npos)
		it = mime_types.find(file.substr(pos)); 
	else
		it = mime_types.end();
	if (it != mime_types.end())
	{
        this->headers[C_TYPE] = it->second;
    }
	else
	{
       this-> headers[C_TYPE]  = "application/octet-stream"; // Default MIME type for unknown files
    }
}


Response::Response(Request req, server_config server) : server(server), req(req)
{
	gen_res(req);
}

Response::~Response()
{
	
}