/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-houd <mel-houd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 05:25:13 by mel-houd          #+#    #+#             */
/*   Updated: 2024/07/21 10:40:48 by mel-houd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Request.hpp"


Request::Request(char *req, server_config server) : request(req), server(server), valid_req(true), status_code(200), status("Ok")
{

}

void	Request::set_req_prop(std::string status, int status_code)
{
	this->valid_req = false;
	this->status = status;
	this->status_code = status_code;
}

void	Request::check_headers_value()
{
	if (this->headers[HOST] != this->server.host + ":" + itos(this->server.port)) // checking host header 
	{
		set_req_prop("Bad Request", BAD_REQ);
		return ;
	}

	if (this->headers[CONNECTION] != "close") // https://developer.mozilla.org/en-US/docs/Web/HTTP/Headers/Connection - the header is not mandatory
		this->headers[CONNECTION] = "keep-alive";

	if (this->headers.find(CONT_LEN) != headers.end()) // check content-length
	{
		for (int i = 0; i < headers[CONT_LEN].size(); i++)
		{
			if (headers[CONT_LEN][i] < '0' || headers[CONT_LEN][i] > '9')
			{
				set_req_prop("Bad Request", BAD_REQ);
				return ;
			}
		}
	}
	// we have 3 essential headers for the server to process the request properly
}

void	Request::parse_req_line()
{
	if (req_line.size() != 3 || find_item(this->server.methods, req_line[0]) == -1) // req line should containe 3 elems and invalid method both have same code
	{
		set_req_prop("Bad Request", BAD_REQ);
		return ;
	}
	if (this->server.routes.find(req_line[1]) == this->server.routes.end()) // url check
	{
		set_req_prop("Not Found", NOT_FOUND);
		return ;
	}
	if (req_line[2] != HTTP_V) //HTTP/1.1
	{
		set_req_prop("HTTP Version Not Supported", HTTP_V_NSUP);
		return ;
	}
}

void	Request::parse_header(std::string& buffer)
{
	std::vector<std::string> header = split(buffer, ':');
	std::string	value;

	if (header.size() < 2 || header[0].find(" ") != std::string::npos) // check if key containes spaces or header is 1 token
	{
		set_req_prop("Bad Request", BAD_REQ);
		return ;
	}
	for (int i = 1; i < header.size(); i++)
	{
		value.append(header[i]);
		if (i != header.size() - 1)
			value.append(":"); // re-append the colon if more parts are present
	}

	value = trim(value);
	string_lower(value);
	string_lower(header[0]);
	this->headers[header[0]] = value;
}

void	Request::parse_req() // we normalise headers and request line to lower case because http is case insensitive
{
	std::string buffer;
	std::istringstream ss(this->request);

	if (std::getline(ss, buffer)) // request line parse
	{
		buffer = trim(buffer);
		string_lower(buffer);
		req_line = split(buffer, ' ');
		string_upper(req_line[2]); // upper the http version
		parse_req_line();
	}
	if (!this->valid_req) return ;
	while (std::getline(ss, buffer)) // headers parser
	{
		if (buffer.size() == 1) // found /r/n in the end of headers
			break ;
		parse_header(buffer);
		if (!this->valid_req) return ;
	}
	while (std::getline(ss, buffer)) // body (need modification if chunked content type is there) - raed until end or depende on content length ??
		this->body.append(buffer);
	check_headers_value();
}

void	Request::print_req()
{
	std::cout << "\nrequest line :\n";
	std::cout << this->req_line[0] << " " << this->req_line[1] << " " << this->req_line[2] << "\n";
	std::cout << "\nheaders :\n";
	for (std::map<std::string, std::string>::const_iterator it = this->headers.begin(); it != this->headers.end(); ++it)
	{
		std::cout << it->first << " : " << it->second << "\n";
	}
	std::cout << "\nbody :\n";
	if (this->body.empty())
	{
		std::cout << "empty body\n";
	}
	else
	{
		std::cout << "body :\n";
		std::cout << this->body << "\n";
	}
}

Request::~Request()
{

}