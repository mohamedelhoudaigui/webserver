/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Helper.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-houd <mel-houd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/13 05:30:53 by mel-houd          #+#    #+#             */
/*   Updated: 2024/10/13 05:35:10 by mel-houd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Include/Helper.hpp"


std::string	TrimAll(std::string Line)
{
	int	start = 0;
	int end = Line.size();
	while (Line[start] && (Line[start] == ' ' || Line[start] == '\t'))
		start++;
	if (start == end)
		return "";
	while (Line[end] && (Line[end] == ' ' || Line[end] == '\t'))
		end--;
	return (Line.substr(start, end - start + 1));
}