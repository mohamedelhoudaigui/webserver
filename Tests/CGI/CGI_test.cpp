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


void	TestCGI()
{
	CGI						cgi;
	HttpRequestBuilder		request_builder;
	std::string 			request;
	HttpRequest*			Req;
    
    request = "GET /index.html HTTP/1.1\r\n";
    request += "Host: www.example.com\r\n";
    request += "User-Agent: CustomBrowser/1.0\r\n";
    request += "Accept: text/html\r\n";
    request += "Connection: close\r\n";
    request += "\r\n";

	request_builder.parseRequest(request);
	request_builder.setBody("zbi hadi body\r\nzbi hadi body\r\nzbi hadi body\r\nzbi hadi body\r\nzbi hadi body\r\nzbi hadi body\r\n");
	Req = request_builder.build();
	cgi.CGISetup(*Req);

	std::cout << cgi.GetResponse() << std::endl;
	std::cout << cgi.GetError() << std::endl;
}