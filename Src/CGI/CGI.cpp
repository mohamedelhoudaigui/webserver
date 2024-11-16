/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGI.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-houd <mel-houd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/14 23:48:05 by mel-houd          #+#    #+#             */
/*   Updated: 2024/11/16 05:25:28 by mel-houd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../Include/CGI.hpp"

CGI::CGI()
{
	
}

void CGI::CGISetup(cgi_params& Params)
{
    const char    *env[8];
    
    env[0] = Params.SERVER_NAME.c_str();
    env[1] = Params.SERVER_PORT.c_str();
    env[2] = Params.REMOTE_ADDR.c_str();
    env[3] = Params.HTTP_USER_AGENT.c_str();
    env[4] = Params.REQUEST_METHOD.c_str();
    env[5] = Params.QUERY_STRING.c_str();
    env[6] = Params.PATH_INFO.c_str();
    env[7] = Params.SCRIPT_NAME.c_str();
    this->env = env;

    Execute();
}

void CGI::Execute() {
    pid_t pid = fork();

    if (pid == 0)
    {
        execve(env[6], const_cast<char **>(&env[6]), const_cast<char **>(env));
        std::cout << "Error executing CGI" << std::endl;
    }
    else if (pid > 0)
    {
        int status;
        waitpid(pid, &status, 0);
    }
    else
    {
        std::cout << "Error Forking CGI" << std::endl;
    }
}