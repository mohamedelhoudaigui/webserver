/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Overloads.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-houd <mel-houd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/14 18:34:06 by mel-houd          #+#    #+#             */
/*   Updated: 2024/10/14 18:39:58 by mel-houd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../Include/Config.hpp"

std::ostream&	operator<<(std::ostream& o, ConfigLines& c)
{
	for (int i = 0; i < c.TokenLines.size(); ++i)
	{
		std::cout << "Line " << i << " :" << std::endl;
		for (int j = 0; j < c.TokenLines[i].Tokens.size(); ++j)
		{
			std::cout << "	" <<
			c.TokenLines[i].Tokens[j].Type << " " <<
			c.TokenLines[i].Tokens[j].Token << std::endl;
		}
	}
	return o;
}

std::ostream&	operator<<(std::ostream& o, RouteConf& r)
{
	o << "		Index : " << r.Index << std::endl;
	o << "		Redir : " << r.Redir << std::endl;
	o << "		AutoIndex : " << r.AutoIndex << std::endl;
	o << "		Loaction : " << r.Location << std::endl;
	o << "		Methods : ";
	for (int i = 0; i < r.Methods.size(); ++i)
	{
		o << r.Methods[i] << " ";
	}
	o << std::endl;
	return o;
}

std::ostream&	operator <<(std::ostream& o, ServerConf& s)
{
	o << "	Port :" << s.Port << std::endl;
	o << "	Root folder : " << s.Root << std::endl;
	o << "	Server name : " << s.ServerName << std::endl;
	for (int i = 0; i < s.Routes.size(); ++i)
	{
		o << "\n=========Route==========" << std::endl;
		o << s.Routes[i];
		o << "========================" << std::endl;
	}
	return o;
}

std::ostream&	operator<<(std::ostream& o, Config& c)
{
	ConfigFile	res = c.GetResult();
	o << "Error page : " << res.ErrorPage << std::endl;
	o << "Max clients : " << res.MaxClients << std::endl;
	o << "Max client body size : " << res.MaxClientBody << std::endl;
	std::cout << "Servers :" << std::endl;
	for (int i = 0; i < res.servers.size(); ++i)
	{
		o << "\n---------Server---------" << std::endl;
		o << res.servers[i];
		o << "-----------------------" << std::endl;
	}
	return o;
}
