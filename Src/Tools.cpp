#include "../Include/Tools.hpp"

void	CheckDigit(std::string& Token, std::string ConfName)
{
    for (size_t i = 0; i < Token.size(); ++i)
    {
        if (!std::isdigit(Token[i]))
            throw std::runtime_error(ConfName + ": " + Token + " is not a valid number value");
    }
}

std::string to_upper(const std::string& str)
{
    std::string upper_str;

    for (size_t i = 0; i < str.size(); ++i)
        upper_str += std::toupper(str[i]);
    return upper_str;
}

std::string TrimAll(const std::string& Line)
{
    size_t start = 0;
    while (start < Line.size() && (Line[start] == ' ' || Line[start] == '\t' || Line[start] == '\n' || Line[start] == '\r'))
        ++start;

    if (start == Line.size())
        return "";

    // Find the last non-whitespace character
    size_t end = Line.size() - 1;
    while (end > start && (Line[end] == ' ' || Line[end] == '\t' || Line[end] == '\n' || Line[end] == '\r'))
    {
        --end;
    }

    // Extract the substring from start to end (inclusive)
    return Line.substr(start, end - start + 1);
}

// checks and gets the value(unsigned int) from a TokenLine
unsigned int	PairValueNum(std::vector<Token>& Tokens, std::string ConfName, unsigned int Maxvalue)
{
	if (Tokens.size() != 2)
		throw std::runtime_error(ConfName + ": need one value");

	std::string& Value = Tokens[1].Token;
    CheckDigit(Value, ConfName);
	long long	Ret = std::atoll(Tokens[1].Token.c_str());
	if (Ret > Maxvalue || Ret < 0)
		throw std::runtime_error(ConfName + ": invalid value " + Value);

	return static_cast<unsigned int>(Ret);
}

std::string	PairValueStr(std::vector<Token>& Tokens, std::string ConfName)
{
	if (Tokens.size() != 2)
		throw std::runtime_error(ConfName + ": need one value");
	return Tokens[1].Token;
}

std::vector<std::string>	MultiValueStr(std::vector<Token>& Tokens, std::string ConfName)
{
	std::vector<std::string>	Values;
	if (Tokens.size() <= 1)
		throw std::runtime_error(ConfName + ": expecting value(s)");
	for (size_t i = 1; i < Tokens.size(); ++i)
	{
		Values.push_back(TrimAll(Tokens[i].Token));
	}
	return (Values);
}

void	MultiValueNum(std::vector<Token>& Tokens, std::string ConfName, std::vector<unsigned int>& Buffer, unsigned int Maxvalue)
{
	if (Tokens.size() <= 1)
		throw std::runtime_error(ConfName + ": expecting value(s)");

	for (size_t i = 1; i < Tokens.size(); ++i)
	{
		CheckDigit(Tokens[i].Token, ConfName);
		std::string& Value = Tokens[i].Token;
		long long	Ret = std::atoll(Tokens[i].Token.c_str());
		if (Ret > Maxvalue || Ret < 0)
			throw std::runtime_error(ConfName + ": invalid value " + Value);
		Buffer.push_back(static_cast<unsigned int>(Ret));
	}
}

bool	PairValueBool(std::vector<Token>& Tokens, std::string ConfName)
{
	if (Tokens.size() != 2)
		throw std::runtime_error(ConfName + ": need one value");

	std::string& value = Tokens[1].Token;
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
	std::string& File = Tokens.back().Token;
	for (size_t i = 1; i < Tokens.size() - 1; ++i)
	{
        CheckDigit(Tokens[i].Token, "ErrorCode");
		unsigned int ErrorCode = static_cast<unsigned int>(atoll(Tokens[i].Token.c_str()));
		if (ErrorCode >= 600)
			throw std::runtime_error("ErrorCode: invalid value " + Tokens[i].Token);
		ErrorPage[ErrorCode] = File;
	}
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

void	Logger(LOG_STATE s, std::string log_msg)
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
	for (size_t i = 0; i < sockets.size(); ++i)
	{
		close(sockets[i]);
	}
}

void    print_ascii(const std::string& str)
{
    for (size_t i = 0; i < str.size(); ++i)
        std::cout << static_cast<int>(str[i]) << " ";
    std::cout << std::endl;
}