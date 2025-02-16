#pragma once

#include <string>
#include <limits.h>
#include <stdexcept>
#include <cstdlib>
#include <fstream>
#include <sys/stat.h>
#include <iostream>
#include <unistd.h>
#include <fcntl.h>

#include "Types.hpp"


std::string					TrimAll(const std::string& Line);
std::string					PairValueStr(std::vector<Token>& Tokens, std::string ConfName);
std::string                 to_upper(const std::string& str);
std::vector<std::string>	MultiValueStr(std::vector<Token>& Tokens, std::string ConfName);
unsigned int				PairValueNum(std::vector<Token>& Tokens, std::string ConfName, unsigned int Maxvalue);
void	                    MultiValueNum(std::vector<Token>& Tokens, std::string ConfName, std::vector<unsigned int>& Buffer, unsigned int Maxvalue);
bool						PairValueBool(std::vector<Token>& Tokens, std::string ConfName);
void						ParseErrorPage(std::vector<Token>& Tokens, std::map<unsigned int, std::string>& ErrorPage);
void						CheckFile(std::string& file, std::string Directive);
void						CheckFolder(std::string& folder, std::string Directive);
void						CheckDigit(std::string& Token, std::string ConfName);
int	                        SetNonBlocking(int fd);
void	                    close_sockets(std::vector<unsigned int>& sockets);
void                        Logger(LOG_STATE s, std::string log_msg);
void                        print_ascii(const std::string& str);
