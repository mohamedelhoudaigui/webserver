/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Tools.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-houd <mel-houd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/13 05:31:43 by mel-houd          #+#    #+#             */
/*   Updated: 2024/11/17 09:09:37 by mel-houd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HELPER_HPP
#define HELPER_HPP

#include <string>
#include <limits.h>
#include <stdexcept>
#include <cstdlib>
#include <fstream>
#include <sys/stat.h>
#include <iostream>
#include <unistd.h>
#include <fcntl.h>

#include "../Include/Types.hpp"


std::string					TrimAll(const std::string& Line);
unsigned int				PairValueNum(std::vector<Token>& Tokens, std::string ConfName, unsigned int Maxvalue);
void	                    MultiValueNum(std::vector<Token>& Tokens, std::string ConfName, std::vector<unsigned int>& Buffer, unsigned int Maxvalue);
std::string					PairValueStr(std::vector<Token>& Tokens, std::string ConfName);
std::vector<std::string>	MultiValueStr(std::vector<Token>& Tokens, std::string ConfName);
bool						PairValueBool(std::vector<Token>& Tokens, std::string ConfName);
void						ParseErrorPage(std::vector<Token>& Tokens, std::map<unsigned int, std::string>& ErrorPage);
void						CheckFile(std::string& file, std::string Directive);
void						CheckFolder(std::string& folder, std::string Directive);
void						CheckDigit(std::string& Token, std::string ConfName);
int	                        SetNonBlocking(int fd);
void	                    close_sockets(std::vector<unsigned int>& sockets);
void                        Logger(LOG_STATE s, std::string log_msg);
void                        print_ascii(const std::string& str);

#endif
