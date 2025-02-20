#pragma once

#include <string>
#include <vector>
#include <map>
#include <stdexcept>
#include <algorithm>
#include <stdio.h>

#define MAX_HEADER_VALUE_LENGTH 4096
#define MAX_HEADER_NAME_LENGTH 256
#define MAX_BODY_SIZE 1048576
#define MAX_HEADER_COUNT 100
#define MAX_PORT 65535
#define MAX_EVENTS 3000


#define RESET   "\033[0m"
#define BLUE    "\033[34m"
#define YELLOW  "\033[33m"
#define GREEN   "\033[32m"
#define RED     "\033[31m"
#define VIOLET  "\033[35m"


// -------------- macro functions:
#define CHECK_ERROR(condition, code, msg) \
    if (condition) {                      \
        SetResultTrue(code, msg);         \
        return;                           \
    }

// -------------- logger types :

enum LOG_STATE
{
	INFO,
	DEBUG,
	WARNING,
	FATAL,
};


// ------------- parsing and  config files types :
enum Scope
{
    GLOBAL = 0,
    SERVER = 1,
    LOCATION = 2,
};

enum TokenType
{
    KEY = 0,
    VALUE = 1,
    OPEN = 2,
    CLOSE = 3,
};

typedef struct Token
{
	std::string	Token;
	TokenType	Type;

}	Token;

typedef struct TokenLine
{
	std::vector<Token>	Tokens;
}	TokenLine;



typedef struct ConfigLines
{
	std::vector<TokenLine>	TokenLines;
}			ConfigLines;
