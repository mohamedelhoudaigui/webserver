/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpResponse.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-houd <mel-houd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/29 19:25:15 by mel-houd          #+#    #+#             */
/*   Updated: 2024/12/04 18:26:47 by mel-houd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTPRESPONSE_HPP
#define HTTPRESPONSE_HPP

#include <string>
#include <map>
#include <iostream>

#include "HttpRequest.hpp"

//product:

class	HttpResponse
{
	private:
		std::string							HttpVersion;
		unsigned int						StatusCode;
		std::string							Status;
		std::map<std::string, std::string>	Headers;
		std::string							Body;
	
	public:
		void	SetHttpVersion(const std::string& HttpVersion);
		void	SetStatusCode(unsigned int StatusCode);
		void	SetStatus(const std::string& Status);
		void	SetHeaders(std::map<std::string, std::string>& Headers); // Add template arguments
		void	SetBody(const std::string& Body);
};

// abstrct builder :

class	AHttpResponseBuilder
{
	protected:
		HttpResponse*	Result;
	public:
		AHttpResponseBuilder();
		virtual ~AHttpResponseBuilder();
		HttpResponse*	GetResult();
		virtual void	BuildHttpVersion() = 0;
		virtual void	BuildStatusCode() = 0;
		virtual void	BuildStatus() = 0;
		virtual void	BuildHeader() = 0;
		virtual void	BuildBody() = 0;
};

// concrete builder :

class	HttpResponseBuilder : public AHttpResponseBuilder
{
	public:
		void	SetRequest(Request& Request);
		void	BuildHttpVersion();
		void	BuildStatusCode();
		void	BuildStatus();
		void	BuildHeader();
		void	BuildBody();
	private:
		Request	Request;
};


#endif