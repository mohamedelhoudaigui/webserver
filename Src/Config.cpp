/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-houd <mel-houd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/13 04:29:25 by mel-houd          #+#    #+#             */
/*   Updated: 2024/10/13 06:21:35 by mel-houd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Include/Config.hpp"


Config::Config(std::string FileName): File(FileName)
{
	if (!File.is_open())
		throw std::runtime_error("Error opening config file");
}

void	Config::Init()
{
	Keys["Global"] = 1;
	Keys["ClientMaxBodySize"] = 1;
	Keys["ErrorPage"] = 1;
	Keys["ServerName"] = 1;
	Keys["Listen"] = 1;
	Keys["Root"] = 1;
	Keys["Location"] = 1;
	Keys["Methods"] = 1;
	Keys["Index"] = 1;
	Keys["Server"] = 1;
	Keys["Redir"] = 1;
	Keys["AutoIndex"] = 1;
	Keys["MaxClients"] = 1;
	Keys["DefaultMethods"] = 1;
	Keys["AutoIndex"] = 1;
}

void	Config::Parse()
{
	std::string	Buffer;

	while (std::getline(File, Buffer))
	{
		Buffer = TrimAll(Buffer);
		if (Buffer != "" && Buffer[0] != '#')
			Tokenise(TrimAll(Buffer));
	}
	File.close();
}



void	Config::Tokenise(std::string LineStr)
{
	TokenLine			Line;
	std::string			Buffer;
	std::stringstream	s(LineStr);
	bool				Key = true;
	
	while (std::getline(s, Buffer, ' '))
	{
		Token	token;
		if (Buffer == "}")
		{
			token.Token = Buffer;
			token.Type = CLOSE;
		}
		else if (Buffer == "{")
		{
			token.Token = Buffer;
			token.Type = OPEN;
		}
		else if (Key && Buffer != "}" && Buffer != "{")
		{
			if (Keys[Buffer] != 1)
			{
				throw std::runtime_error("Invalid key in config file :" + Buffer);
			}
			token.Token = Buffer;
			token.Type = KEY;
			Key = false;
		}
		else
		{
			token.Token = Buffer;
			token.Type = VALUE;
		}
		Line.Tokens.push_back(token);
	}
	ConfLines.TokenLines.push_back(Line);
}

ConfigLines	Config::GetLines()
{
	return (this->ConfLines);
}

//-- overload :

std::ostream&	operator<<(std::ostream& o, ConfigLines& c)
{
	for (int i = 0; i < c.TokenLines.size(); ++i)
	{
		std::cout << "Line " << i << " :" << std::endl;
		for (int j = 0; j < c.TokenLines[i].Tokens.size(); ++j)
		{
			std::cout << "	" << c.TokenLines[i].Tokens[j].Type << " " << c.TokenLines[i].Tokens[j].Token << std::endl;
		}
	}
	return o;
}