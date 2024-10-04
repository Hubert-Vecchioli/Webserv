/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hvecchio <hvecchio@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/27 16:31:05 by hvecchio          #+#    #+#             */
/*   Updated: 2024/10/04 06:22:31 by hvecchio         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

Server::Server() : _isServerGreenlighted(false), _serverFD(-1)
{
	this->_uniqueInstance = *this;
}

Server::~Server()
{
	if (this->_serverFD != -1)
		close(_serverFD);
	//close and delete all
}

void Server::startServer(ConfigurationFile & configurationFile)
{
	print(1, "[Info] - Initialising the webserv");
	this->_serverFD(epoll_create(MAX_EVENTS));
    if (this->_serverFD == -1)
		throw FailureInitiateEpollInstanceException();
	std::vector<pair <std::string ip, unsigned int port> ip_port>  = configurationFile.getIpPorts();
	for (std::vector<BlocServer>::iterator it = parsed_config.begin(); it != parsed_config.end(); ++it)
	{
		if (socket(AF_INET, SOCK_STREAM, 0) == -1)
			throw FailureInitiateSocketException();
		epoll_event ev;
		ev.events = EPOLLRDHUP | EPOLLHUP | EPOLLERR | EPOLLIN;
		ev.data.fd = blocServersFD;
		if (epoll_ctl(this->_serverFD, EPOLL_CTL_ADD, blocServersFD, &ev) == -1)
			throw FailureAddFDToEpollException();
		this->_sockets[blocServersFD] = new Socket(blocServersFD); // rendre la socket non bloquante avec fcntl()
	}
	this->_isServerGreenlighted = true;
	print(1, "[Info] - Webserv initialised");
}

void Server::runServer(void)
{
	epoll_event	epollEvents[MAX_EVENTS];
	try
	{
		print(1, "[Info] - Webserv started");
		while(this->_isServerGreenlighted == true)
		{
			if (epoll_wait(this->_serverFD, epollEvents, MAX_EVENTS, EPOLL_MAX_WAIT_TIME_MS) == -1)
				throw FailureEpollWaitException();
				// Should we stop the project ? I chose to but to be reviewed
			for (int eventId = 0; eventId < epollWaitResult; eventId++)
				triageEvents(epollEvents, eventId);
		}
	}
	catch(const std::exception& e)
	{
		print(2, e.what());
	}
}

void Server::stop( void )
{
	this->_isServerGreenlighted(false);
}

void Server::triageEvents(epoll_event *epollEvents, int eventId)
{
	if (event & (EPOLLHUP | EPOLLERR | EPOLLRDHUP))
		//Disconnect the client and print a message
	if (event & EPOLLIN)
		// Check it the client exists: Add a new client or manage the received request
		// EPOLLOUT?
}

Server & getInstance(void)
{
	// static method to return the only instance generated
}

const char* Server::FailureInitiateEpollInstanceException::what() const throw()
{
	return ("[Error] - Failure to initiate the epoll instance");
}

const char* Server::FailureInitiateSocketException::what() const throw()
{
	return ("[Error] - Failure to initiate a socket while initialising servers");
}

const char* Server::FailureAddFDToEpollException::what() const throw()
{
	return ("[Error] - Failure to add the FD to the epoll listen list");
}

const char* Server::FailureEpollWaitException::what() const throw()
{
	return ("[Error] - Failure to add the FD to the epoll listen list");
}