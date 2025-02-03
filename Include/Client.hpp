#pragma once

#include "HttpRequest.hpp"
#include <string>
#include <sys/socket.h>
#include <iostream>
#include <unistd.h>
#include <fstream>
#include <sstream>
#include <ctime>

class Client
{
private:
	int             fd;
	std::string     request_buffer;
	std::string     response;
	Request*        current_request;
	size_t          requests_handled;
	time_t          last_activity;
	bool            keep_alive;
	static const size_t MAX_REQUESTS = 100;
	static const time_t TIMEOUT = 5; // 5 seconds

	void processRequest();
	void buildResponse();
	bool hasTimedOut() const;
	void updateLastActivity();

public:
	Client();
	~Client();
	Client(const Client& other);
	Client& operator=(const Client& other);
	
	int     Recv(int BufferSize);
	void    Send();
	void    SetFd(int fd);
	int     GetFd() const;
	bool    shouldClose() const;
	void    reset();
};
