/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGI.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-houd <mel-houd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/14 23:48:05 by mel-houd          #+#    #+#             */
/*   Updated: 2024/12/09 20:23:06 by mel-houd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../Include/CGI.hpp"


CGI::CGI()
{

}

CGI::~CGI()
{

}

void CGI::CGISetup(Request& request)
{
	if (pipe(this->stdin_pipe) == -1)
		std::cerr << "CGI: Error piping stding" << std::endl;
	if (pipe(this->stdout_pipe) == -1)
		std::cerr << "CGI: Error piping stdout" << std::endl;

    // set cgi output and error into nonBlocking
    SetNonBlocking(stdout_pipe[0]);
}

int CGI::GetStdoutFd()
{
    return (stdout_pipe[0]);
}

void CGI::ReadPipe(int pipe_fd, std::string& s)
{
    char	buffer[1024];
    ssize_t	bytesRead;

    while ((bytesRead = read(pipe_fd, buffer, sizeof(buffer) - 1)) > 0)
    {
        buffer[bytesRead] = '\0';
        s.append(buffer);
		bzero(buffer, 1024); // clear buffer after read
    }
    close(pipe_fd);
}

void CGI::WritePipe(int pipe_fd, const std::string& s)
{
    write(pipe_fd, s.c_str(), strlen(s.c_str()));
    close(pipe_fd);
}

std::string CGI::convert_header_name(const std::string& header_name)
{
    std::string converted = to_upper(header_name);
    std::replace(converted.begin(), converted.end(), '-', '_');
    return ("HTTP_" + converted);
}

std::vector<std::string>	CGI::PrepareEnv(Request& request)
{
	std::vector<std::string>    env;
	const std::map<std::string, std::string>& headers = request.getHeaders();
	std::map<std::string, std::string>::const_iterator	it;

    env.push_back("REQUEST_METHOD=" + request.getMethod());
    env.push_back("SCRIPT_NAME=" + request.getPath());
    env.push_back("QUERY_STRING=" + request.getQuery());
    env.push_back("SERVER_PROTOCOL=" + request.getHttpVersion());
    env.push_back("CONTENT_LENGTH=" + request.getContentLength());
    // env.push_back("CONTENT_TYPE=" + request.getContentType()); // not implemented yet
	for (it = headers.begin(); it != headers.end(); ++it)
	{
		env.push_back(convert_header_name(it->first) + "=" + it->second);
	}
	return (env);
}

void CGI::Execute(Request& request)
{
	pid_t   ProcId = fork();
	if (ProcId == 0)
	{
		// setup executable for execve:
		char	*argv[2];
		argv[0] = strdup(request.getPath().c_str()); // change it to SCRIPT_NAME
		argv[1] = NULL;

        std::vector<std::string> env = PrepareEnv(request);
		std::vector<char *>			execve_env;
		for (int i = 0; i < env.size(); ++i)
		{
			execve_env.push_back(const_cast<char *>(env[i].c_str()));
		}
		execve_env.push_back(NULL);

		// prepare pipes:
		dup2(stdin_pipe[0], STDIN_FILENO);
		dup2(stdout_pipe[1], STDOUT_FILENO);

		// clean up:
		close(this->stdin_pipe[0]);
        close(this->stdin_pipe[1]);
		close(this->stdout_pipe[0]);
        close(this->stdout_pipe[1]);

		execve(argv[0], argv, &execve_env[0]);
		// in case execve failed:
		exit(1);
	}
	else if (ProcId > 0)
	{
		close(this->stdin_pipe[0]);
		close(this->stdout_pipe[1]);

		// write Request Body in the STDIN
		WritePipe(this->stdin_pipe[1], request.getBody());

		// read output:
		ReadPipe(this->stdout_pipe[0], this->CgiResponse);

		// need to be removed (add the output file to the kqueue)
		waitpid(ProcId, NULL, WNOHANG);
	}
	else
	{
		Logger(FATAL, "Error Forking CGI");
	}
}

std::string&	CGI::GetResponse()
{
	return (CgiResponse);
}

