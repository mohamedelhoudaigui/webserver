/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-houd <mel-houd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/13 04:29:25 by mel-houd          #+#    #+#             */
/*   Updated: 2024/11/17 15:47:44 by mel-houd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../Include/Config.hpp"


Config::Config(std::string FileName)
{
	File.open(FileName.c_str());
	if (!File.is_open())
		throw std::runtime_error("Error opening config file");

	GlobalKeys.push_back("DefaultErrorPage");
	GlobalKeys.push_back("Server");
	GlobalKeys.push_back("DefaultIndex");
	GlobalKeys.push_back("DefaultUploadDir");
	GlobalKeys.push_back("DefaultRoot");
	GlobalKeys.push_back("DefaultMaxClientBody");

	ServerKeys.push_back("Listen");
	ServerKeys.push_back("ServerName");
	ServerKeys.push_back("ErrorPage");
	ServerKeys.push_back("Location");
	ServerKeys.push_back("Host");

	LocationKeys.push_back("UploadDir");
	LocationKeys.push_back("Index");
	LocationKeys.push_back("Redir");
	LocationKeys.push_back("AutoIndex");
	LocationKeys.push_back("DirList");
	LocationKeys.push_back("Root");
	LocationKeys.push_back("Methods");
	LocationKeys.push_back("IsCgi");
}

void	Config::Parse() // main parser
{
	Tokeniser();
	CheckSyntaxError();
	AssignTokens();
	CheckResult();
}

ConfigLines	Config::GetLines()
{
	return (this->ConfLines);
}

ConfigFile	Config::GetResult()
{
	return (this->Result);
}

std::fstream&	Config::GetLog()
{
	return (this->File);
}