/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Helper.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-houd <mel-houd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/13 05:31:43 by mel-houd          #+#    #+#             */
/*   Updated: 2024/10/13 14:27:37 by mel-houd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HELPER_HPP
#define HELPER_HPP

#include <string>
#include <limits.h>
#include <stdexcept>
#include <cstdlib>
#include "../Include/Types.hpp"


std::string					TrimAll(const std::string& Line);
std::string					PairValueStr(std::vector<Token>& Tokens, std::string ConfName);
unsigned int				PairValueNum(std::vector<Token>& Tokens, std::string ConfName);
std::vector<std::string>	MultiValueStr(std::vector<Token>& Tokens, std::string ConfName);
bool						PairValueBool(std::vector<Token>& Tokens, std::string ConfName);

#endif