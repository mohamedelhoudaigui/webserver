/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-houd <mel-houd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/13 04:29:25 by mel-houd          #+#    #+#             */
/*   Updated: 2024/10/14 18:51:21 by mel-houd         ###   ########.fr       */
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