/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Gen_dir.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-houd <mel-houd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/26 11:32:09 by mel-houd          #+#    #+#             */
/*   Updated: 2024/07/26 11:59:23 by mel-houd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GEN_DIR_HPP
#define GEN_DIR_HPP

#include <iostream>
#include <dirent.h>
#include <sys/types.h>
#include <map>
#include <vector>


class	Gen_dir
{
	public:
		Gen_dir(std::string dir_path);
		~Gen_dir();
		std::map<std::string, std::string> MIME_types; // html -> text/html
		std::map<std::string, std::vector<std::string> >	files; // url -> (filename - mime type)
		void	generate();

	private:
		std::string	dir_path;
};


#endif