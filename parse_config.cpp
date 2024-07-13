/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_config.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-houd <mel-houd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/12 23:52:36 by mel-houd          #+#    #+#             */
/*   Updated: 2024/07/13 10:16:00 by mel-houd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parse_config.hpp"


void	handle_line(std::string line, server_config& server)
{
	std::stringstream ss(line);
	std::string	tmp;

	while (getline(ss, tmp, ':'))
	{
		if (tmp == "listen")
		{
			getline(ss, tmp, ':');
			server.port = atoi(tmp.c_str());
			if (server.port < MIN_PORT || server.port > MAX_PORT)
			{
				std::cerr << "invalid port number : " << server.port << "\n";
				exit(1);
			}
			break ;
		}
		if (tmp == "server_name")
		{
			getline(ss, tmp, ':');
			server.host = tmp;
			break ;
		}
		if (tmp == "error_pages")
		{
			bool	key = true;
			int		err_code;
			while (getline(ss, tmp, ':'))
			{
				if (key)
				{
					err_code = atoi(tmp.c_str());
					if (err_code < MIN_STATUS || err_code > MAX_STATUS)
					{
						std::cerr << "invalid status code number : " << err_code << "\n";
						exit(1);
					}
					key = false;
				}
				else if (!key)
				{
					key = true;
					std::string path = tmp;
					if (open(tmp.c_str(), O_RDONLY, 0) == -1)
					{
						std::cerr << "invalid status code file : " << path << "\n";
						exit(1);
					}
					server.error_pages[err_code] = path;		
				}
			}
			break ;
		}
		if (tmp == "routes")
		{
			bool		key = true;
			std::string	route;
			while (getline(ss, tmp, ':'))
			{
				if (key)
				{
					route = tmp;
					if (route[0] != '/')
					{
						std::cerr << "invalid route : " << route << "\n";
						exit(1);
					}
					key = false;
				}
				else if (!key)
				{
					key = true;
					std::string path;
					int	i;

					for (i = 0; i < tmp.size(); i++)
					{
						if (tmp[i] == '(')
						{
							i++;
							break ;
						}
						path.push_back(tmp[i]);
					}
					if (open(path.c_str(), O_RDONLY, 0) == -1)
					{
						std::cerr << "invalid route file : " << path << "\n";
						exit(1);
					}
					std::string					method;
					std::vector<std::string>	path_methods;
					path_methods.push_back(path);
					while (tmp[i] && tmp[i] != ')')
					{
						while (tmp[i] && tmp[i] != ')' && tmp[i] != ',')
						{
							method.push_back(tmp[i]);
							i++;
						}
						if (method != POST || method != GET || method != DELETE)
						{
							std::cerr << "invalid method : " << method << "\n";
							exit(1);
						}
						path_methods.push_back(method);
						method.clear();
						i++;
					}
					server.routes[route] = path_methods;		
				}
			}
			break ;			
		}
		if (tmp == "body_size")
		{
			getline(ss, tmp, ':');
			server.body_size = (unsigned int)atoi(tmp.c_str());
			if (server.body_size > MAX_BODY_SIZE)
			{
				std::cerr << "max body size exeeded\n";
				exit(1);
			}
			break ;
		}
	}
}

//---------------

Parse_config::Parse_config(std::string file_name): file_strm(file_name)
{
	if(!file_strm.is_open())
	{
		std::cerr << "failed to open file : \n" << file_name << "\n";
		exit(1);
	}
}

void	Parse_config::parse()
{
	std::string	line;
	
	try
	{
		while (std::getline(this->file_strm, line))
		{
			// std::string	trimmed;
			// int			i = 0;

			// while (i < line.size())
			// {
			// 	if (line[i] != SPACE || line[i] != TAB)
			// 		break ;
			// 	i++;
			// }
			// while (i < line.size())
			// {
			// 	if (line[i] == SPACE || line[i] == TAB)
			// 		break ;
			// 	trimmed.push_back(line[i]);
			// 	i++;
			// }
			if (line == "server:")
			{
				server_config server;
				server.port = -1;
				this->servers.push_back(server);
			}
			else
				handle_line(line, this->servers[this->servers.size() - 1]);
		}
	}
	catch(const std::exception& e)
	{
		std::cerr << "exeption in parse_config.cpp :\n" << e.what() << '\n';
		exit(1);
	}
	

	for (int i = 0; i < this->servers.size(); i++)
	{
		if (servers[i].port == -1 || servers[i].host.empty() || servers[i].routes.empty() || servers[i].error_pages.empty())
		{
			std::cerr << "missing args in config file\n";
			exit(1);
		}
	}
}




Parse_config::~Parse_config()
{
	
}
