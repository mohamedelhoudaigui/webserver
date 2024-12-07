/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGI.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-houd <mel-houd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/14 23:48:05 by mel-houd          #+#    #+#             */
/*   Updated: 2024/12/07 02:57:25 by mel-houd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../Include/CGI.hpp"

CGI::CGI()
{

}

CGI::~CGI()
{
	
}

void CGI::CGISetup(cgi_params &Params, std::string& Request, std::string& execBinary)
{
	this->Params = Params;

	if (pipe(this->stdin_pipe) == -1)
		std::cout << "CGI: Error piping stding" << std::endl;
	Execute(execBinary);
}

void CGI::ReadPipe(int pipe_fd, std::string& s)
{
    char buffer[1024];
    ssize_t bytesRead;

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

void CGI::Execute(std::string& execBinary)
{
	ProcId = fork();
	if (ProcId == 0)
	{
		char		*env[9];
		char		*argv[3];
	
		env[0] = strdup(std::string("SERVER_NAME=" + Params.SERVER_NAME).c_str());
		env[1] = strdup(std::string("SERVER_PORT=" + Params.SERVER_PORT).c_str());
		env[2] = strdup(std::string("REMOTE_ADDR=" + Params.REMOTE_ADDR).c_str());
		env[3] = strdup(std::string("HTTP_USER_AGENT=" + Params.HTTP_USER_AGENT).c_str());
		env[4] = strdup(std::string("REQUEST_METHOD=" + Params.REQUEST_METHOD).c_str());
		env[5] = strdup(std::string("QUERY_STRING=" + Params.QUERY_STRING).c_str());
		env[6] = strdup(std::string("PATH_INFO=" + Params.PATH_INFO).c_str());
		env[7] = strdup(std::string("SCRIPT_NAME=" + Params.SCRIPT_NAME).c_str());
		env[8] = NULL;

		argv[0] = strdup(execBinary.c_str());
		argv[1] = strdup(Params.PATH_INFO.c_str());
		argv[2] = NULL;

		dup2(stdin_pipe[0], STDIN_FILENO);

		FdOut = open("./CGIout", O_CREAT | O_WRONLY | O_TRUNC, 0777);
		dup2(FdOut, STDOUT_FILENO);
		close(FdOut);


		close(this->stdin_pipe[0]);
        close(this->stdin_pipe[1]);

		execve(argv[0], argv, env);
	}
	else if (ProcId > 0)
	{
		close(this->stdin_pipe[0]);
		WritePipe(this->stdin_pipe[1], this->Params.ReqBody);
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