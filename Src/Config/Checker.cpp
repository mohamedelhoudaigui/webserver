/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Checker.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-houd <mel-houd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/14 18:33:16 by mel-houd          #+#    #+#             */
/*   Updated: 2024/10/15 19:41:01 by mel-houd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../Include/Config.hpp"

void	Config::CheckResult()
{
	CheckGlobalParams();
	CheckServers();
}

//assign default values if none are provided except default error page
void	Config::CheckGlobalParams()
{
	// check error page existense
	std::fstream	f;
	f.open(Result.ErrorPage.c_str());
	if (!f.good())
		throw std::runtime_error(Result.ErrorPage + ": Default error page doesn't exist");
	f.close();
}


//check server data
void	Config::CheckServers()
{
	std::vector<ServerConf>	Servers = Result.servers;
	for (std::vector<ServerConf>::iterator it = Servers.begin(); it != Servers.end(); ++it)
	{
		if (it->Port == 0
			|| it->Root.empty()
			|| it->ServerName.empty()
			|| it->Host.empty()
		)
		{
			throw std::runtime_error("Server params error: invalid parameters");
		}
		CheckLocations(it->Routes, *it);
	}
}

void	Config::CheckLocations(std::vector<RouteConf>& Locations, ServerConf& Server)
{
	for (std::vector<RouteConf>::iterator it = Locations.begin(); it != Locations.end(); ++it)
	{
		if (it->Index.empty())
		{
			if (it->AutoIndex == true)
				it->Index = "index.html";
			else
				throw std::runtime_error("Location params error: no index specified");
		}
		else
		{
			std::string	FullPath = "./www/" + Server.Root + "/" + it->Index;
			std::fstream	f;
			f.open(FullPath);
			if (!f.good())
				throw std::runtime_error("Location params error: index file not found: " + FullPath);
		}
		for (std::vector<std::string>::iterator mt = it->Methods.begin(); mt != it->Methods.end(); ++mt)
		{
			if (*mt != "GET" && *mt != "POST" && *mt != "DELETE")
				throw std::runtime_error("Location params error: invalid HTPP method: " + *mt);
		}
		// how to check redirection ??
	}
}
