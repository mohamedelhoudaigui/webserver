/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-houd <mel-houd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/12 02:06:41 by mel-houd          #+#    #+#             */
/*   Updated: 2024/10/12 06:14:09 by mel-houd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Include/Response.hpp"


Response::Response(Request& req): req(req)
{}

void	Response::Parse()
{
	BuildSLine();
	ExtractResource(req.GetResult().ReqLine.Path);
	AppendHeaders();
	
}

void	Response::BuildSLine()
{
	this->res.Stline.HttpVersion = req.GetResult().ReqLine.HttpVersion;
	this->res.Stline.StatusCode = req.GetResult().Status;
	this->res.Stline.StatusText = "OK";
}

void	Response::AppendHeaders()
{
	int		ContentLen = res.Body.size();
	std::stringstream	s(ContentLen);
	std::string	ContentLenStr;
	s >> ContentLenStr;

	res.Headers["Content-Type"] = "text/html";
	res.Headers["Content-Length"] = ContentLenStr;
		
}

void	Response::ExtractResource(std::string Path)
{
	std::string	File = "./www" + Path;
	if (File == "./www/" || File == "./www")
		File = "./www/index.html";

	std::ifstream file(File);

	if (!file.is_open())
	{
		std::cerr << "Error opening file: " << File << std::endl;
		return ;
	}

	std::string Line;
	while (std::getline(file, Line)) {
		res.Body.append(Line);
	}

	file.close();
}
void	Response::AssembleRes()
{
	std::string StatusLine = res.Stline.HttpVersion + " " + res.Stline.StatusCode + " " + res.Stline.StatusText + "\n";
	res.BufferRes.append(StatusLine);
	for (std::map<std::string, std::string>::iterator it = res.Headers.begin(); it != res.Headers.end(); ++it)
	{
		res.BufferRes.append(it->first + ": " + it->second + "\n");
	}
	res.BufferRes.append("\r\n\r\n");
	res.BufferRes.append(res.Body);
	res.RawResponse = res.BufferRes.c_str();
}
