/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Getparser.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mboutuil <mboutuil@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 03:17:26 by mboutuil          #+#    #+#             */
/*   Updated: 2024/11/15 17:22:22 by mboutuil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include<request.hpp>

void    Getparser::parse_headers(std::string headers)
{
    std::string tmp;
    int index;
    tmp = headers.substr(0,headers.find_first_of("\r\n"));
    while (!tmp.empty())
    {
        
    }
}