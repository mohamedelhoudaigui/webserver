/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Getters.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-houd <mel-houd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/16 18:51:31 by mel-houd          #+#    #+#             */
/*   Updated: 2024/11/17 15:50:29 by mel-houd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../Include/Types.hpp"


//global Getters :

bool			ConfigFile::CheckServer(std::string& Host, unsigned int Port)
{
	std::vector<ServerConf>::iterator it;
	std::vector<ServerConf>& Servers = this->servers;

	for (it = Servers.begin(); it != Servers.end(); ++it)
	{
		if (it->Host == Host && Port == it->Port)
			return (true);
	}
	return (false);
}

ServerConf&	ConfigFile::GetServer(std::string& Host, unsigned int Port)
{
	std::vector<ServerConf>::iterator it;
	std::vector<ServerConf>& Servers = this->servers;

	for (it = Servers.begin(); it != Servers.end(); ++it)
	{
		if (it->Host == Host && Port == it->Port)
			return (*it);
	}
	throw std::runtime_error("server not found");
}

// default getters:

std::string&	DefaultConf::GetDefaultErrorPage()
{
	return (this->DefaultErrorPage);
}


std::string&	DefaultConf::GetDefaultRoot()
{
	return (this->DefaultRoot);
}


std::string&	DefaultConf::GetDefaultIndex()
{
	return (this->DefaultIndex);
}


std::string&	DefaultConf::GetDefaultUploadDir()
{
	return (this->DefaultUploadDir);
}


unsigned int	DefaultConf::GetDefaultMaxBody()
{
	return (this->DefaultMaxClientBody);
}

//ServerGetters :

std::string&	ServerConf::GetHost()
{
	return (this->Host);
}


std::string&	ServerConf::GetServerName()
{
	return (this->ServerName);
}

std::string&		ServerConf::GetErrorPage(unsigned int ErrorCode)
{
	std::map<unsigned int, std::string>::iterator it;
	it = this->ErrorPage.find(ErrorCode);
	if (it == this->ErrorPage.end())
		return (this->Default->GetDefaultErrorPage());
	return (it->second);
}

bool	ServerConf::CheckLocation(std::string& LocationPath)
{
	std::vector<RouteConf>::iterator route;
	for (route = this->Routes.begin(); route != this->Routes.end(); ++route)
	{
		if (LocationPath == route->Location)
			return (true);
	}
	return (false);
}

RouteConf&	ServerConf::GetLocation(std::string& LocationPath)
{
	std::vector<RouteConf>::iterator route;
	for (route = this->Routes.begin(); route != this->Routes.end(); ++route)
	{
		if (LocationPath == route->Location)
			return (*route);
	}
	throw std::runtime_error("NotFound");
}


// Location Getters:

unsigned int	RouteConf::GetMaxBody()
{
	if (this->MaxClientBody == 0)
		return (this->Default->GetDefaultMaxBody());
	return (this->MaxClientBody);
}

bool	RouteConf::CheckDirectoryListing()
{
	return (this->DirList);
}

bool	RouteConf::CheckAutoIndex()
{
	return (this->AutoIndex);
}

bool	RouteConf::CheckRedirection()
{
	if (this->Redir.empty())
		return (false);
	return (true);
}

bool	RouteConf::CheckUploadDir()
{
	if (UploadDir.empty())
		return (false);
	return (true);
}

bool	RouteConf::CheckMethod(std::string& Method)
{
	std::vector<std::string>::iterator it;
	it = find(this->Methods.begin(), this->Methods.end(), Method);
	if (it == this->Methods.end())
		return (false);
	return (true);
}

std::string&	RouteConf::GetRoot()
{
	if (this->Root.empty())
		return (this->Default->GetDefaultRoot());
	return (this->Root);
}

std::string&	RouteConf::GetUploadDir()
{
	if (this->UploadDir.empty())
		return (this->Default->GetDefaultUploadDir());
	return (this->UploadDir);
}

std::string&	RouteConf::GetIndex()
{
	if (this->Index.empty())
		return (this->Default->GetDefaultIndex());
	return (this->Index);
}

std::string&	RouteConf::GetRedirection()
{
	if (this->Redir.empty())
		throw std::runtime_error("NotFound");
	return (this->Redir);
}

std::string&	RouteConf::GetLocationPath()
{
	return (this->Location);
}

bool	RouteConf::CheckIsCgi()
{
	return (this->IsCgi);
}
