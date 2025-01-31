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

void CGI::CGISetup(Request &request)
{
	Error.clear();
	Response.clear();

	if (pipe(this->stdin_pipe) == -1)
		std::cerr << "CGI: Error piping stding" << std::endl;
	if (pipe(this->stderr_pipe) == -1)
		std::cerr << "CGI: Error piping stderr" << std::endl;
	if (pipe(this->stdout_pipe) == -1)
		std::cerr << "CGI: Error piping stdout" << std::endl;
	Execute(request);
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


std::vector<std::string>	CGI::PrepareEnv(Request &request)
{
	std::vector<std::string>						env;
	std::map<std::string, std::string>				headers;
	std::map<std::string, std::string>::iterator	it;

	// headers = request.getHeaders();
	env.push_back("GATEWAY_INTERFACE=CGI/1.1");
	env.push_back("SERVER_PROTOCOL=HTTP/1.1");
	for (it = headers.begin(); it != headers.end(); ++it)
	{
		env.push_back(it->first + "=" + it->second);
	}
	return (env);
}

void CGI::Execute(Request &request)
{
	ProcId = fork();
	if (ProcId == 0)
	{
		// setup executable for execve:
		char	*argv[2];
		argv[0] = strdup("./Tests/CGI/python_script");
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
		dup2(stderr_pipe[1], STDOUT_FILENO);
		dup2(stderr_pipe[1], STDERR_FILENO);

		// clean up:
		close(this->stdin_pipe[0]);
        close(this->stdin_pipe[1]);
		close(this->stdout_pipe[0]);
        close(this->stdout_pipe[1]);
        close(this->stderr_pipe[1]);
        close(this->stderr_pipe[1]);

		execve(argv[0], argv, &execve_env[0]);
		// in case execve failed:
		exit(1);
	}
	else if (ProcId > 0)
	{
		close(this->stdin_pipe[0]);
		close(this->stderr_pipe[1]);
		close(this->stdout_pipe[1]);

		// write Request Body in the STDIN
		WritePipe(this->stdin_pipe[1], request.getBody());

		// read Any errors from STDERR (better for debuging)
		ReadPipe(this->stderr_pipe[0], this->Error);

		// read output:
		ReadPipe(this->stdout_pipe[0], this->Response);

		// need to be removed (add the output file to the kqueue)
		waitpid(ProcId, NULL, 0);
	}
	else
	{
		std::cerr << "Error Forking CGI" << std::endl;
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