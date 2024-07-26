/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   convert.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-houd <mel-houd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/19 01:26:22 by mel-houd          #+#    #+#             */
/*   Updated: 2024/07/26 11:25:31 by mel-houd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/Utils.hpp"

std::string itos(int value)
{
	std::ostringstream s;
	s << value;
	return (s.str());
}

std::string	ctos(char *buffer)
{
	std::string	buf(buffer);
	return (buf);
}

void string_lower(std::string &str)
{
    for (size_t i = 0; i < str.size(); ++i)
	{
        str[i] = tolower(static_cast<unsigned char>(str[i]));
    }
}

void	string_upper(std::string &str)
{
    for (size_t i = 0; i < str.size(); ++i)
	{
        str[i] = toupper(static_cast<unsigned char>(str[i]));
    }
}

std::vector<std::string> split(std::string buffer, char delimiter)
{
	std::vector<std::string> result;
	std::string token;
	std::istringstream tokenStream(buffer);

	while (std::getline(tokenStream, token, delimiter))
	{
		if (!token.empty())
		{
			result.push_back(token);
		}
	}
	return (result);
}

std::string	trim(std::string& s)
{
	size_t start = s.find_first_not_of(" \t\r\n");
	size_t end = s.find_last_not_of(" \t\r\n");
	return(s.substr(start, end - start + 1));
}

int	find_item(std::vector<std::string> vector, std::string needle)
{
	for (int i = 0; i < vector.size(); i++)
	{
		if (vector[i] == needle)
			return (i);
	}
	return (-1);
}
