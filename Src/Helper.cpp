/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Helper.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-houd <mel-houd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/13 05:30:53 by mel-houd          #+#    #+#             */
/*   Updated: 2024/10/13 09:45:04 by mel-houd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Include/Helper.hpp"

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

unsigned int	PairValueNum(std::vector<Token>& Tokens, std::string ConfName)
{
	if (Tokens.size() != 2)
		throw std::runtime_error(ConfName + ": more than 1 value");

	std::string	Value = Tokens[1].Token;
	for (std::string::iterator it = Value.begin(); it != Value.end(); ++it)
	{
        if (!std::isdigit(*it))
            throw std::runtime_error(ConfName + ": invalid value " + Value);
    }

	long long	Ret = std::atoll(Tokens[1].Token.c_str());
	if (Ret > UINT32_MAX)
		throw std::runtime_error(ConfName + ": invalid value" + Value);

	return Ret;
}

std::string	PairValueStr(std::vector<Token>& Tokens, std::string ConfName)
{
	if (Tokens.size() != 2)
		throw std::runtime_error(ConfName + ": more than 1 value");
    
	std::string	value = Tokens[1].Token;
	return value;
}