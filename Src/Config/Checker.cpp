/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Checker.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-houd <mel-houd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/14 18:33:16 by mel-houd          #+#    #+#             */
/*   Updated: 2024/10/24 04:07:56 by mel-houd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../Include/Config.hpp"

void	Config::CheckResult()
{
	CheckGlobalParams();
	CheckServers();
	CheckServerNames();
}

//assign default values if none are provided except default error page
void	Config::CheckGlobalParams()
{
	// check files existense
	CheckFile(Result.Default.DefaultErrorPage);
	CheckFile(Result.Default.DefaultIndex);
	CheckFolder(Result.Default.DefaultRoot);
	CheckFolder(Result.Default.DefaultUploadDir);
	if (Result.Default.DefaultMaxClients == 0 ||
		Result.Default.DefaultMaxClientBody == 0)
	{
		throw std::runtime_error("missing or invalid default configuration (DefaultMaxClients or DefaultMaxClientBody)");
	}
	
}

//Check uniques ServerNames

void	Config::CheckServerNames()
{
	std::vector<ServerConf>::iterator	it;
	std::vector<std::string>			ServerNames;

	for (it = Result.servers.begin(); it != Result.servers.end(); ++it)
		ServerNames.push_back(it->ServerName);

	sort(ServerNames.begin(), ServerNames.end());
	std::string							tmp;

	for (int i = 0; i < ServerNames.size(); ++i)
	{
		if (i != 0 && tmp == ServerNames[i])
			throw std::runtime_error("Duplicated ServerName attribute");
		tmp = ServerNames[i];
	}
}


//check server data
void	Config::CheckServers()
{
	std::vector<ServerConf>	Servers = Result.servers;
	for (std::vector<ServerConf>::iterator it = Servers.begin(); it != Servers.end(); ++it)
	{
		if (it->Port.size() == 0
			|| it->ServerName.empty()
			|| it->Host.empty()
		)
		{
			throw std::runtime_error("Server params error: invalid parameters");
		}
		CheckPorts(it->Port);
		CheckLocations(it->Routes, *it);
	}
}

void	Config::CheckLocations(std::vector<RouteConf>& Locations, ServerConf& Server)
{
	for (std::vector<RouteConf>::iterator it = Locations.begin(); it != Locations.end(); ++it)
	{
		CheckIndex(*it);
		CheckMethods(*it);
		CheckFolder(it->Root);
	}
}

void	Config::CheckPorts(std::vector<unsigned int>& Ports)
{
	std::vector<unsigned int>::iterator it;

	for (it = Ports.begin(); it != Ports.end(); ++it)
	{
		if (*it > 65535)
			throw std::runtime_error("Invalid port number");
	}
}

void	Config::CheckIndex(RouteConf& Location)
{
	if (Location.Index.empty())
	{
		if (Location.AutoIndex == true)
			Location.Index = "index.html";
		else
			throw std::runtime_error("Location params error: no index specified");
	}
	else
	{
		std::string	FullPath = Location.Root + "/" + Location.Index;
		CheckFile(FullPath);
	}
}


void	Config::CheckMethods(RouteConf& Loaction)
{
	std::vector<std::string>::iterator it;
	for (it = Loaction.Methods.begin(); it !=Loaction.Methods.end(); ++it)
	{
		if (*it != "GET" && *it != "POST" && *it != "DELETE" && *it != "PUT")
			throw std::runtime_error("Location params error: invalid HTPP method: " + *it);
	}
}