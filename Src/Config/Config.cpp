/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-houd <mel-houd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/13 04:29:25 by mel-houd          #+#    #+#             */
/*   Updated: 2024/10/16 03:45:43 by mel-houd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../Include/Config.hpp"


Config::Config(std::string FileName)
{
	File.open(FileName.c_str());
	if (!File.is_open())
		throw std::runtime_error("Error opening config file");
}

void	Config::Init() // check Key token with key words
{
	GlobalKeys.push_back("DefaultErrorPage");
	GlobalKeys.push_back("Server");

	ServerKeys.push_back("Listen");
	ServerKeys.push_back("ServerName");
	ServerKeys.push_back("ErrorPage");
	ServerKeys.push_back("Location");
	ServerKeys.push_back("Host");
	ServerKeys.push_back("MaxClients");

	LocationKeys.push_back("ClientMaxBodySize");
	LocationKeys.push_back("Root");
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
	//CheckResult();
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