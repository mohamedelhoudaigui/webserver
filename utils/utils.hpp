/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-houd <mel-houd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/19 01:26:42 by mel-houd          #+#    #+#             */
/*   Updated: 2024/07/21 10:34:24 by mel-houd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTILS_HPP
#define UTILS_HPP

#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>

std::string					itos(int value);
std::string					ctos(char *buffer);
std::string 				trim(std::string& s);
std::vector<std::string>	split(std::string buffer, char delimiter);
void						string_lower(std::string& input);
void						string_upper(std::string &str);
int							find_item(std::vector<std::string> vector, std::string needle);


#endif