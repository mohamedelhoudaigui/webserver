/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HtppResponse.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-houd <mel-houd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/29 19:56:40 by mel-houd          #+#    #+#             */
/*   Updated: 2024/12/04 18:27:30 by mel-houd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HttpResponse.hpp"

//-------------------------product------------------------

void	HttpResponse::SetHttpVersion(const std::string& HttpVersion)
{
	this->HttpVersion = HttpVersion;
}

void    HttpResponse::SetStatusCode(unsigned int StatusCode)
{
	this->StatusCode = StatusCode;
}

void	HttpResponse::SetStatus(const std::string& Status)
{
	this->Status = Status;
}

void    HttpResponse::SetHeaders(std::map<std::string, std::string>& Headers)
{
	this->Headers = Headers;
}

void	HttpResponse::SetBody(const std::string& Body)
{
	this->Body = Body;
}

//-------------------------asbtract builder------------------------

AHttpResponseBuilder::AHttpResponseBuilder()
{
	this->Result = new HttpResponse();
}

AHttpResponseBuilder::~AHttpResponseBuilder()
{}

HttpResponse*	AHttpResponseBuilder::GetResult()
{
	return (this->Result);
}


//----------------------concrete builder-------------------

void	HttpResponseBuilder::SetRequest(Request& Request)
{
	this->Request = Request;
}

void	HttpResponseBuilder::BuildHttpVersion()
{
	this->Result->SetHttpVersion("HTTP/1.1");
}

void	HttpResponseBuilder::BuildStatusCode()
{
	
}