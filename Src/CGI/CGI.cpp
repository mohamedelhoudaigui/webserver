/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGI.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-houd <mel-houd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/14 23:48:05 by mel-houd          #+#    #+#             */
/*   Updated: 2024/11/16 11:08:18 by mel-houd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../Include/CGI.hpp"

CGI::CGI()
{

}

CGI::~CGI()
{
	
}

void CGI::CGISetup(cgi_params& Params, std::string Request)
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
	this->Request = Request;

	pipe(this->stdin_pipe);
	pipe(this->stdout_pipe);
	pipe(this->stderr_pipe);

	Execute();
}

void CGI::ReadPipe(int pipe_fd, std::string& s)
{
    char buffer[1024];
    ssize_t bytesRead;

    // Read from stdout pipe
    while ((bytesRead = read(pipe_fd, buffer, sizeof(buffer) - 1)) > 0)
    {
        buffer[bytesRead] = '\0';
        s.append(buffer);
    }
    close(pipe_fd);
}

void CGI::WritePipe(int pipe_fd, std::string& s)
{
    write(pipe_fd, s.c_str(), strlen(s.c_str()));
    close(pipe_fd);
}

void CGI::Execute()
{
	ProcId = fork();

	if (ProcId == 0)
	{
		dup2(stdin_pipe[0], STDIN_FILENO);
		dup2(stdout_pipe[1], STDOUT_FILENO);
		dup2(stderr_pipe[1], STDERR_FILENO);

		close(this->stdin_pipe[0]);
        close(this->stdin_pipe[1]);

        close(this->stdout_pipe[0]);
        close(this->stdout_pipe[1]);

        close(this->stderr_pipe[0]);
        close(this->stderr_pipe[1]);


		execve(env[6], const_cast<char **>(&env[6]), const_cast<char **>(env));
		std::cout << "Error executing CGI" << std::endl;
		exit(1);
	}
	else if (ProcId > 0)
	{
		close(this->stdin_pipe[0]);
		close(this->stdout_pipe[1]);
		close(this->stderr_pipe[1]);

		WritePipe(this->stdin_pipe[1], this->Request);
		ReadPipe(this->stdout_pipe[0], this->Response);
		ReadPipe(this->stderr_pipe[0], this->Error);

		waitpid(ProcId, NULL, 0);
	}

	else
	{
		std::cout << "Error Forking CGI" << std::endl;
	}
}

std::string&	CGI::GetResponse()
{
	return (Response);
}

std::string&	CGI::GetError()
{
	return (Error);
}