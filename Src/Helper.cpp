/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Helper.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-houd <mel-houd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/13 05:30:53 by mel-houd          #+#    #+#             */
/*   Updated: 2024/10/23 23:22:01 by mel-houd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Include/Helper.hpp"

// trims spaces tabs and \n from the end and the beginning
std::string TrimAll(const std::string& Line)
{
    std::string::const_iterator start = Line.begin();
    std::string::const_iterator end = Line.end();

    while (start != end && (*start == ' ' || *start == '\t' || *start == '\n'))
        ++start;

    if (start == end)
        return "";

    --end;
    while (end != start && (*end == ' ' || *end == '\t' || *end == '\n'))
        --end;

    return (Line.substr(start - Line.begin(), end - start + 1));
}

// checks and gets the value(unsigned int) from a TokenLine
unsigned int	PairValueNum(std::vector<Token>& Tokens, std::string ConfName)
{
	if (Tokens.size() != 2)
		throw std::runtime_error(ConfName + ": need one value");

	std::string	Value = Tokens[1].Token;
	for (std::string::iterator it = Value.begin(); it != Value.end(); ++it)
	{
        if (!std::isdigit(*it))
            throw std::runtime_error(ConfName + ": invalid value " + Value);
    }

	long long	Ret = std::atoll(Tokens[1].Token.c_str());
	if (Ret > UINT_MAX)
		throw std::runtime_error(ConfName + ": invalid value" + Value);

	return Ret;
}

// checks and gets the value(std::string) from a TokenLine
std::string	PairValueStr(std::vector<Token>& Tokens, std::string ConfName)
{
	if (Tokens.size() != 2)
		throw std::runtime_error(ConfName + ": need one value");
    
	std::string	value = Tokens[1].Token;
	return value;
}


std::vector<std::string>	MultiValueStr(std::vector<Token>& Tokens, std::string ConfName)
{
	std::vector<std::string>	Values;
	if (Tokens.size() <= 1)
		throw std::runtime_error(ConfName + ": expecting value(s)");
	for (int i = 1; i < Tokens.size(); ++i)
	{
		Values.push_back(TrimAll(Tokens[i].Token));
	}
	return (Values);
}

std::vector<unsigned int>	MultiValueNum(std::vector<Token>& Tokens, std::string ConfName)
{
	std::vector<unsigned int>	Values;

	if (Tokens.size() <= 1)
		throw std::runtime_error(ConfName + ": expecting value(s)");

	for (int i = 1; i < Tokens.size(); ++i)
	{
		std::string	Value = Tokens[i].Token;
		for (std::string::iterator it = Value.begin(); it != Value.end(); ++it)
		{
			if (!std::isdigit(*it))
				throw std::runtime_error(ConfName + ": invalid value " + Value);
		}
		
		long long	Ret = std::atoll(Tokens[1].Token.c_str());
		if (Ret > UINT_MAX)
			throw std::runtime_error(ConfName + ": invalid value " + Value);

		Values.push_back((unsigned int)Ret);
	}
	return Values;
}

bool	PairValueBool(std::vector<Token>& Tokens, std::string ConfName)
{
	if (Tokens.size() != 2)
		throw std::runtime_error(ConfName + ": need one value");
    
	std::string	value = Tokens[1].Token;
	if (value != "on" && value != "off")
		throw std::runtime_error(ConfName + ": invalid value (on or off)");
	if (value == "on")
		return true;
	return false;
}


void	ParseErrorPage(std::vector<Token>& Tokens, std::map<unsigned int, std::string>& ErrorPage)
{
	if (Tokens.size() <= 2)
		throw std::runtime_error("ErrorPage: expecting value(s)");
	std::string	File = Tokens.back().Token;
	for (int i = 1; i < Tokens.size() - 1; ++i)
	{
		unsigned int ErrorCode = static_cast<unsigned int>(atoll(Tokens[i].Token.c_str()));
		if (ErrorCode >= 600)
			throw std::runtime_error("ErrorCode: invalid value " + Tokens[i].Token);
		ErrorPage[ErrorCode] = File;
	}
}