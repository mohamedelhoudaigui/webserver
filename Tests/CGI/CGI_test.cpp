/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGI_test.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-houd <mel-houd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/16 05:09:11 by mel-houd          #+#    #+#             */
/*   Updated: 2024/11/16 11:08:04 by mel-houd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../../Include/CGI.hpp"

void	TestCGI()
{
	CGI c;
	std::string	script = "/Users/mel-houd/Desktop/webserver/Tests/CGI/script.sh";

	cgi_params p;
	p.PATH_INFO = script;
	std::string	Request = "hello world\n";

	c.CGISetup(p, Request);

	std::cout << c.GetResponse() << std::endl;
}