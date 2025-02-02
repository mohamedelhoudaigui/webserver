/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGI_test.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-houd <mel-houd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/16 05:09:11 by mel-houd          #+#    #+#             */
/*   Updated: 2024/12/09 21:18:27 by mel-houd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../Include/CGI.hpp"

// deprecated
void	TestCGI()
{
	CGI						c;
	Request		request_builder;
	std::string 			request;
	Request*			Req;
    
    request = "GET /index.html HTTP/1.1\r\n";
    request += "Host: www.example.com\r\n";
    request += "User-Agent: CustomBrowser/1.0\r\n";
    request += "Accept: text/html\r\n";
    request += "Connection: close\r\n";
    request += "\r\n";

	request_builder.parseRequest(request);
	// request_builder.setBody("zbi hadi body\r\nzbi hadi body\r\nzbi hadi body\r\nzbi hadi body\r\nzbi hadi body\r\nzbi hadi body\r\n");
	// Req = request_builder.build();
	c.CGISetup(*Req);

	std::cout << c.GetResponse() << std::endl;
	std::cout << c.GetError() << std::endl;
}
