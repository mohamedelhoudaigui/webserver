/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-houd <mel-houd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 05:25:13 by mel-houd          #+#    #+#             */
/*   Updated: 2024/07/19 08:24:24 by mel-houd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Request.hpp"

//----------------tools---------------

void	string_lower(std::string& input)
{
	std::string result = input;
	for (size_t i = 0; i < result.size(); ++i)
	{
		if (result[i] >= 'A' && result[i] <= 'Z')
			result[i] += 'a' - 'A';
	}
}

std::vector<std::string> split(std::string &buffer, char delimiter)
{
	std::vector<std::string> result;
	std::string token;
	std::istringstream tokenStream(buffer);

	while (std::getline(tokenStream, token, delimiter))
	{
		if (!token.empty())
		{
			result.push_back(token);
		}
	}
	return (result);
}

std::string trim(std::string &s)
{
	size_t start = s.find_first_not_of(" \t\r\n");
	size_t end = s.find_last_not_of(" \t\r\n");
	return (s.substr(start, end - start + 1));
}

int	find_item(std::vector<std::string>& vector, std::string& needle)
{
	for (int i = 0; i < vector.size(); i++)
	{
		if (vector[i] == needle)
			return (i);
	}
	return (-1);
}

//---------------Main---------------------


Request::Request(char *req, server_config server) : request(req), server(server), valid_req(true)
{

}

void	Request::parse_req_line()
{
	if (req_line.size() != 3 || find_item(this->server.methods, req_line[0]) == -1) // req line should containe 3 elems and invalid method both have same code
	{
		this->valid_req = false;
		this->err = "Bad Request";
		this->err_code = BAD_REQ;
		return ;
	}
	if (find_item(this->server.routes, req_line[1]) == -1) // url check
	{
		this->valid_req = false;
		this->err = "Not Found";
		this->err_code = NOT_FOUND;
		return ;
	}
	if (req_line[2] != HTTP_V) //HTTP/1.1
	{
		this->valid_req = false;
		this->err = "HTTP Version Not Supported";
		this->err_code = HTTP_V_NSUP;
		return ;
	}
}

void	Request::parse_header(std::string& buffer)
{
	std::vector<std::string> header = split(buffer, ':');
	if (header.size() < 2 || header[0].find(" ") != std::string::npos) // check if key containes spaces or header is 1 token
	{
		this->valid_req = false;
		this->err = "Bad Request";
		this->err_code = BAD_REQ;
	}
	std::string	value;
	for (int i = 1; i < header.size(); i++)
	{
		value.append(header[i]);
		if (i != header.size() - 1)
		{
			value.append(":"); // re-append the colon if more parts are present
		}
	}
	value = trim(value);
	string_lower(value);
	string_lower(header[0]);
	this->headers[header[0]] = value;
}

void	Request::parse_req()
{
	std::string buffer;
	std::istringstream ss(this->request);

	if (std::getline(ss, buffer)) // request line parse
	{
		buffer = trim(buffer);
		req_line = split(buffer, ' ');
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
	while (std::getline(ss, buffer)) // body (need modification if chunked content type is there) :
	{
		this->body.append(buffer);
	}
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