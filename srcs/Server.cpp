/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hvecchio <hvecchio@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/27 16:31:05 by hvecchio          #+#    #+#             */
/*   Updated: 2024/10/02 19:36:22 by hvecchio         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

Server::Server() : _isServerGreenlighted(false), _serverFD(-1)
{
}

Server::~Server()
{
	if (this->_serverFD != -1)
		close(_serverFD);
	//close and delete all
}

void Server::startServer(ConfigurationFile & configurationFile)
{
	print(1, "[Info] - Starting the webserv");
	this->_serverFD(epoll_create(MAX_EVENTS));
    if (this->_serverFD == -1)
		throw FailureInitiateEpollInstanceException();
	//row below to do udated - containers to be modified
	std::vector<std::vector<BlockServer>> &parsed_config = configurationFile.getBlockServers();
	for (std::vector<BlocServer>::iterator it = parsed_config.begin(); it != parsed_config.end(); ++it)
	{
		int blocServersFD = socket(AF_INET, SOCK_STREAM, 0);
		if (blocServersFD == -1)
			throw FailureInitiateSocketException();
		epoll_event ev;
		ev.events = EPOLLRDHUP | EPOLLHUP | EPOLLERR | EPOLLIN;
		ev.data.fd = blocServersFD;
		int epollFDAddResult = epoll_ctl(this->_serverFD, EPOLL_CTL_ADD, blocServersFD, &ev);
		if (epollFDAddResult == -1)
			throw FailureAddFDToEpollException();
		this->_sockets[blocServersFD] = new Socket(blocServersFD, &it);
		// rendre la socket non bloquante avec fcntl()
	}
	this->_isServerGreenlighted = true;
}

void Server::runServer(void)
{
	epoll_event	epollEvents[MAX_EVENTS];
	try
	{
		while(this->_isServerGreenlighted == true)
		{
			int epollWaitResult = epoll_wait(this->_serverFD, epollEvents, MAX_EVENTS, EPOLL_MAX_WAIT_TIME_MS);
			if (epollWaitResult == -1)
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