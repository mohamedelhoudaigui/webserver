/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Tools.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-houd <mel-houd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/13 05:30:53 by mel-houd          #+#    #+#             */
/*   Updated: 2025/01/22 12:09:29 by mel-houd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Include/Tools.hpp"

void	CheckDigit(std::string& Token, std::string& ConfName)
{
    for (int i = 0; i < Token.size(); ++i)
    {
        if (Token[i] < '0' || Token[i] > '9')
            throw std::runtime_error(ConfName + ": " + Token + " is not a valid number value");
    }
}

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
unsigned int	PairValueNum(std::vector<Token>& Tokens, std::string ConfName, long Maxvalue)
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
	if (Ret > Maxvalue)
		throw std::runtime_error(ConfName + ": invalid value " + Value);

	return static_cast<unsigned int>(Ret);
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


void	MultiValueNum(std::vector<Token>& Tokens, std::string ConfName, std::vector<unsigned int>& Buffer)
{
	if (Tokens.size() <= 1)
		throw std::runtime_error(ConfName + ": expecting value(s)");

	for (int i = 1; i < Tokens.size(); ++i)
	{
		CheckDigit(Tokens[i].Token, ConfName); //uncaught
		std::string	Value = Tokens[i].Token;
		long long	Ret = std::atoll(Tokens[i].Token.c_str());
		if (Ret > UINT_MAX)
			throw std::runtime_error(ConfName + ": invalid value " + Value);
		Buffer.push_back(static_cast<unsigned int>(Ret));
	}
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

void	CheckFile(std::string& file, std::string directive)
{
	std::fstream	f;
	f.open(file.c_str());
	if (!f.good())
		throw std::runtime_error(directive + ": " + file + ": doesn't exist");
	f.close();
}

 void	CheckFolder(std::string& folder, std::string Directive)
 {
    struct stat sb;
	if (!(stat(folder.c_str(), &sb) == 0 && S_ISDIR(sb.st_mode)))
		throw std::runtime_error(Directive + ": " + folder + ": doesnt exist");
}



//--------------server

int	SetNonBlocking(int fd)
{
	int flags = fcntl(fd, F_GETFL, 0);
	if (flags < 0)
	{
		close(fd);
		Logger(WARNING, "failed to get fd flags");
		return (-1);
	}

    if (fcntl(fd, F_SETFL, flags | O_NONBLOCK) < 0)
	{
		close(fd);
		Logger(WARNING, "failed to set fd flags to non-blocking");
		return (-1);
	}
	return (0);
}

int EnvLen(char **env)
{
	int	i = 0;
	while (env[i])
		i++;
	return (i);
}

void	Logger(STATE s, std::string log_msg)
{
	switch (s) {
		case INFO:
			std::cout << BLUE << "[INFO]: ";
			break ;
		case DEBUG:
			std::cout << GREEN << "[DEBUG]: ";
			break ;
		case WARNING:
			std::cout << YELLOW << "[WARNING]: ";
			break ;
		case FATAL:
			std::cout << RED << "[FATAL]: ";
			break ;
	}
	std::cout << RESET << log_msg << std::endl;
}

void	close_sockets(std::vector<unsigned int>& sockets)
{
	for (int i = 0; i < sockets.size(); ++i)
	{
		close(sockets[i]);
	}
}