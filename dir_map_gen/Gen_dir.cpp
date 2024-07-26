/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Gen_dir.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-houd <mel-houd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/26 11:19:34 by mel-houd          #+#    #+#             */
/*   Updated: 2024/07/26 12:13:53 by mel-houd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/Gen_dir.hpp"

std::string	get_type(std::string& file)
{
	std::string	ret;
	size_t pos = file.find_last_of(".");
	if (pos == std::string::npos)
	{
		ret = "txt";
		return (ret);
	}
	ret = file.substr(pos);
	return (ret);
}

//---------------------------------------------------------

Gen_dir::Gen_dir(std::string dir_path) : dir_path(dir_path)
{
	this->MIME_types["txt"] = "text/plain";
	this->MIME_types["html"] = "text/html";
	this->MIME_types["json"] = "application/json";
	this->MIME_types["js"] = "application/javascript";
	this->MIME_types["css"] = "text/css";
	this->MIME_types["png"] = "image/png";
	this->MIME_types["jpeg"] = "text/jpeg";
	this->MIME_types["mp4"] = "video/mp4";
}

void	Gen_dir::generate()
{
	DIR*	directory = opendir(this->dir_path.c_str());
	if (!directory)
	{
		std::cerr << "error openning directory " << this->dir_path << "\n";
		exit(1);
	}
	
	struct dirent*	entry;
	while ((entry = readdir(directory)) != NULL)
	{
		std::string	file_name = entry->d_name;
		std::string	type = get_type(file_name);
		std::vector<std::string> name_and_type;;
		name_and_type.push_back(entry->d_name);
		name_and_type.push_back(type);
		this->files["/" + file_name] = name_and_type;
	}
	
	for (std::map<std::string, std::vector<std::string> >::iterator it = files.begin(); it != files.end(); it++)
	{
	}
}

Gen_dir::~Gen_dir()
{
	
}