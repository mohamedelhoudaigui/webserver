/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-houd <mel-houd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/13 04:29:25 by mel-houd          #+#    #+#             */
/*   Updated: 2025/02/16 23:50:41 by mel-houd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../Include/Config.hpp"

Config::Config() {}

Config::Config(std::string FileName)
{
	Logger(INFO, "config file parse:");
	File.open(FileName.c_str());
	if (!File.is_open())
		throw std::runtime_error("Error opening config file");

	Logger(DEBUG, "ready to read config file");

	GlobalKeys.push_back("Server");

	ServerKeys.push_back("Host");
	ServerKeys.push_back("Listen");
	ServerKeys.push_back("ServerName");
	ServerKeys.push_back("Root");
	ServerKeys.push_back("Location");

	LocationKeys.push_back("Index");
	LocationKeys.push_back("AutoIndex");
	LocationKeys.push_back("Methods");
	LocationKeys.push_back("IsCgi");
	LocationKeys.push_back("CgiName");
	LocationKeys.push_back("UploadDir");
	LocationKeys.push_back("DirList");
	LocationKeys.push_back("Redir");
}

Config::~Config() {
	File.close();
}


//--------------------------

void	Config::Parse()
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

ConfigFile&	Config::GetResult()
{
	return (this->Result);
}

std::fstream&	Config::GetLog()
{
	return (this->File);
}
