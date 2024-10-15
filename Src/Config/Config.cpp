/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-houd <mel-houd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/13 04:29:25 by mel-houd          #+#    #+#             */
/*   Updated: 2024/10/15 23:13:34 by mel-houd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../Include/Config.hpp"


Config::Config(std::string FileName)
{
	File.open(FileName.c_str());
	if (!File.is_open())
		throw std::runtime_error("Error opening config file");
	Result.MaxClientBody = 16000; // default values
	Result.MaxClients = 600;
}

void	Config::Init() // check Key token with key words
{
	GlobalKeys.push_back("ClientMaxBodySize");
	GlobalKeys.push_back("ErrorPage");
	GlobalKeys.push_back("MaxClients");
	GlobalKeys.push_back("Server");

	ServerKeys.push_back("Listen");
	ServerKeys.push_back("ServerName");
	ServerKeys.push_back("Root");
	ServerKeys.push_back("Location");
	ServerKeys.push_back("Host");

	LocationKeys.push_back("Methods");
	LocationKeys.push_back("UpDir");
	LocationKeys.push_back("Index");
	LocationKeys.push_back("Redir");
	LocationKeys.push_back("AutoIndex");
	LocationKeys.push_back("DirList");
}



void	Config::Parse() // main parser
{
	Tokeniser();
	CheckSyntaxError();
	AssignTokens();
	CheckResult();
}

//-- getters :

ConfigLines	Config::GetLines()
{
	return (this->ConfLines);
}

ConfigFile	Config::GetResult()
{
	return (this->Result);
}