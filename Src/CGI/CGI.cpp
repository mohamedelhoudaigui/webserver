/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGI.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-houd <mel-houd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/14 23:48:05 by mel-houd          #+#    #+#             */
/*   Updated: 2024/11/17 16:10:08 by mel-houd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../Include/CGI.hpp"

CGI::CGI()
{

}

CGI::~CGI()
{
	
}

void CGI::CGISetup(cgi_params &Params, std::string Request, char **e)
{
	this->Params = Params;

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
		char *env[9];
		MakeEnv(env);

		dup2(stdin_pipe[0], STDIN_FILENO);
		dup2(stdout_pipe[1], STDOUT_FILENO);
		dup2(stderr_pipe[1], STDERR_FILENO);

		close(this->stdin_pipe[0]);
        close(this->stdin_pipe[1]);

        close(this->stdout_pipe[0]);
        close(this->stdout_pipe[1]);

        close(this->stderr_pipe[0]);
        close(this->stderr_pipe[1]);

		execve(Params.PATH_INFO.c_str(), NULL, env);
		std::cout << "Error executing CGI" << std::endl;
		exit(1);
	}
	else if (ProcId > 0)
	{
		close(this->stdin_pipe[0]);
		close(this->stdout_pipe[1]);
		close(this->stderr_pipe[1]);

		WritePipe(this->stdin_pipe[1], this->Params.Request);

		ReadPipe(this->stdout_pipe[0], this->Response);
		ReadPipe(this->stderr_pipe[0], this->Error);

		waitpid(ProcId, NULL, 0);
	}

	else
	{
		std::cout << "Error Forking CGI" << std::endl;
	}
}

void CGI::MakeEnv(char *res[])
{
	res[0] = strdup(std::string("SERVER_NAME=" + Params.SERVER_NAME).c_str());
	res[1] = strdup(std::string("SERVER_PORT=" + Params.SERVER_PORT).c_str());

	res[2] = strdup(std::string("REMOTE_ADDR=" + Params.REMOTE_ADDR).c_str());
	res[3] = strdup(std::string("HTTP_USER_AGENT=" + Params.HTTP_USER_AGENT).c_str());

	res[4] = strdup(std::string("REQUEST_METHOD=" + Params.REQUEST_METHOD).c_str());
	res[5] = strdup(std::string("QUERY_STRING=" + Params.QUERY_STRING).c_str());

	res[6] = strdup(std::string("PATH_INFO=" + Params.PATH_INFO).c_str());
	res[7] = strdup(std::string("SCRIPT_NAME=" + Params.SCRIPT_NAME).c_str());

	res[8] = NULL;
}

std::string&	CGI::GetResponse()
{
	return (Response);
}

std::string&	CGI::GetError()
{
	return (Error);
}