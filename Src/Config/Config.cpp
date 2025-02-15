/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-houd <mel-houd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/13 04:29:25 by mel-houd          #+#    #+#             */
/*   Updated: 2025/01/26 11:16:57 by mel-houd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../Include/Config.hpp"


Config::Config(std::string FileName)
{
	Logger(INFO, "config file parse:");
	File.open(FileName.c_str());
	if (!File.is_open())
		throw std::runtime_error("Error opening config file");

	Logger(DEBUG, "ready to read config file");

	GlobalKeys.push_back("DefaultErrorPage");
	GlobalKeys.push_back("Server");
	GlobalKeys.push_back("ClientBodyLimit");

	ServerKeys.push_back("Listen");
	ServerKeys.push_back("ServerName");
	ServerKeys.push_back("Host");
	ServerKeys.push_back("Location");
	ServerKeys.push_back("Root");

	LocationKeys.push_back("Index");
	LocationKeys.push_back("Redir");
	LocationKeys.push_back("UploadDir");
	LocationKeys.push_back("AutoIndex");
	LocationKeys.push_back("DirList");
	LocationKeys.push_back("Methods");
	LocationKeys.push_back("IsCgi");
	LocationKeys.push_back("CgiName");
}

void	Config::Parse() // main parser
{
	Tokeniser();
	CheckSyntaxError();
	AssignTokens();
	CheckResult();
	Logger(DEBUG, "parsing config file is done");
}

ConfigLines	Config::GetLines()
{
	return (this->ConfLines);
}

ConfigFile& Config::GetResult()
{
	return (this->Result);
}

std::fstream&	Config::GetLog()
{
	return (this->File);
}
