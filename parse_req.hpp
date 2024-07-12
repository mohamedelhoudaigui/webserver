/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_req.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-houd <mel-houd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/11 22:01:42 by mel-houd          #+#    #+#             */
/*   Updated: 2024/07/11 22:17:23 by mel-houd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <vector>
#include <string>
#include <iostream>
#include <map>

class	Parse_req
{
	public:
		Parse_req(std::string req);
		void	split_args();
		~Parse_req();
	private:
		std::string	req;
};