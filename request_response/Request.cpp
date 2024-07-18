/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-houd <mel-houd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 05:25:13 by mel-houd          #+#    #+#             */
/*   Updated: 2024/07/16 11:15:50 by mel-houd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Request.hpp"

std::vector<std::string> split(const std::string &buffer, char delimiter)
{
    std::vector<std::string>	result;
    std::string					token;
    std::istringstream			tokenStream(buffer);

    while (std::getline(tokenStream, token, delimiter))
	{
        result.push_back(token);
    }
    return (result);
}

std::string trim(const std::string &s)
{
	size_t start = s.find_first_not_of(" \t\r\n");
	size_t end = s.find_last_not_of(" \t\r\n");
	return (s.substr(start, end - start + 1));
}

Request::Request(char *req) : request(req)
{
    std::string buffer;
    std::istringstream ss(this->request);

	// req_line :
    if (std::getline(ss, buffer))
	{
        buffer = trim(buffer);
        std::vector<std::string> req_line = split(buffer, ' ');

        if (req_line.size() >= 3)
		{
            this->method = req_line[0];
            this->url = req_line[1];
            this->http_version = req_line[2];
        }
		else
		{
            std::cerr << "Invalid request line\n";
			exit(1);
        }
    }
	// headers :
    while (std::getline(ss, buffer))
	{
		if (buffer.size() == 1)
		{
			break ;
		}
        buffer = trim(buffer);
        std::vector<std::string> header = split(buffer, ':');
		size_t	pos = buffer.find(":");
		pos++;
		buffer = buffer.substr(pos);
		header[0] = trim(header[0]);
		buffer = trim(buffer);
        this->headers[header[0]] = buffer;
    }
	// body :
	while (std::getline(ss, buffer))
	{
		this->body.append(buffer);
	}
}

void	Request::print_req()
{
	std::cout << "\nrequest line :\n";
	std::cout << this->method << " " << this->url << " " << this->http_version << "\n";
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