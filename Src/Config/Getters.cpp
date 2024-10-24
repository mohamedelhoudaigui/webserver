/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Getters.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-houd <mel-houd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/16 18:51:31 by mel-houd          #+#    #+#             */
/*   Updated: 2024/10/24 03:24:05 by mel-houd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../Include/Types.hpp"
#include "../../Include/Exps.hpp"


//global Getters :

bool			ConfigFile::CheckServer(std::string& Host, unsigned int Port)
{
	std::vector<ServerConf>::iterator it;
	std::vector<ServerConf>& Servers = this->servers;

	for (it = Servers.begin(); it != Servers.end(); ++it)
	{
		if (it->Host == Host)
		{
			std::vector<unsigned int>::iterator PortIt;
			for (PortIt = it->Port.begin(); PortIt != it->Port.end(); ++PortIt)	
			{
				if (Port == *PortIt)
				{
					return (true);
				}
			}	
		}
	}
	return (false);
}

ServerConf&	ConfigFile::GetServer(std::string& Host, unsigned int Port)
{
	std::vector<ServerConf>::iterator server;
	std::vector<ServerConf>& Servers = this->servers;

	for (server = Servers.begin(); server != Servers.end(); ++server)
	{
		if (server->Host == Host)
		{
			if (server->CheckPort(Port))
				return (*server);
		}
	}
	throw GetterExc(NotFound);
}

// default getters:





//ServerGetters :

unsigned int	ServerConf::GetMaxClients()
{
	return (this->MaxClients);
}

std::string&	ServerConf::GetHost()
{
	return (this->Host);
}

bool			ServerConf::CheckPort(unsigned int Port)
{
	std::vector<unsigned int>::iterator it;
	it = find(this->Port.begin(), this->Port.end(), Port);
	if (it == this->Port.end())
		return (false);
	return (true);
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
		throw GetterExc(NotFound);
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
	throw GetterExc(NotFound);
}


// Location Getters:

unsigned int	RouteConf::GetMaxBody()
{
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
	return (this->Root);
}

std::string&	RouteConf::GetUploadDir()
{
	if (this->UploadDir.empty())
		throw GetterExc(NotFound);
	return (this->UploadDir);
}

std::string&	RouteConf::GetIndex()
{
	return (this->Index);
}

std::string&	RouteConf::GetRedirection()
{
	if (this->Redir.empty())
		throw GetterExc(NotFound);
	return (this->Redir);
}

std::string&	RouteConf::GetLocationPath()
{
	return (this->Location);
}
