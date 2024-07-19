/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-houd <mel-houd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 05:25:13 by mel-houd          #+#    #+#             */
/*   Updated: 2024/07/19 09:33:29 by mel-houd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Request.hpp"

//----------------tools---------------

void string_lower(std::string &str)
{
    for (size_t i = 0; i < str.size(); ++i)
	{
        str[i] = tolower(static_cast<unsigned char>(str[i]));
    }
}


std::vector<std::string> split(std::string buffer, char delimiter)
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

std::string	trim(std::string& s)
{
	size_t start = s.find_first_not_of(" \t\r\n");
	size_t end = s.find_last_not_of(" \t\r\n");
	return(s.substr(start, end - start + 1));
}

int	find_item(std::vector<std::string> vector, std::string needle)
{
	for (int i = 0; i < vector.size(); i++)
	{
		if (vector[i] == needle)
			return (i);
	}
	return (-1);
}

//---------------Main---------------------


Request::Request(char *req, server_config server) : request(req), server(server), valid_req(true), err_code(200), err("Ok")
{

}

void	Request::set_req_prop(std::string err, int err_code)
{
	this->valid_req = false;
	this->err = err;
	this->err_code = err_code;
}

void	Request::parse_req_line()
{
	if (req_line.size() != 3 || find_item(this->server.methods, req_line[0]) == -1) // req line should containe 3 elems and invalid method both have same code
	{
		set_req_prop("Bad Request", BAD_REQ);
		return ;
	}
	if (find_item(this->server.routes, req_line[1]) == -1) // url check
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

void	Request::parse_req()
{
	std::string buffer;
	std::istringstream ss(this->request);

	if (std::getline(ss, buffer)) // request line parse
	{
		buffer = trim(buffer);
		string_lower(buffer);
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
	if (this->headers[HOST] != this->server.host + ":" + itos(this->server.port)) // checking host header 
	{
		set_req_prop("Bad Request", BAD_REQ);
		return ;
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