/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hvecchio <hvecchio@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/27 16:31:05 by hvecchio          #+#    #+#             */
/*   Updated: 2024/10/04 17:06:05 by hvecchio         ###   ########.fr       */
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
	std::vector<pair <std::string ip, unsigned int port> ip_port> &parsed_config = configurationFile.getIpPorts();
	for (std::vector<BlocServer>::iterator it = parsed_config.begin(); it != parsed_config.end(); ++it)
	{
		if (socket(AF_INET, SOCK_STREAM, 0) == -1)
			throw FailureInitiateSocketException();
		epoll_event ev;
		ev.events = EPOLLRDHUP | EPOLLHUP | EPOLLERR | EPOLLIN;
		ev.data.fd = blocServersFD;
		if (epoll_ctl(this->_serverFD, EPOLL_CTL_ADD, blocServersFD, &ev) == -1)
			throw FailureAddFDToEpollException();
		this->_sockets[blocServersFD] = new Socket(blocServersFD, parsed_config.second, parsed_config.first);
	}
	this->_isServerGreenlighted = true;
	print(1, "[Info] - Webserv initialised");
}

void Server::stop( void )
{
	this->_isServerGreenlighted(false);
}

void Server::runServer(void)
{
	epoll_event	epollEvents[MAX_EVENTS];
	try
	{
		print(1, "[Info] - Webserv started");
		while(this->_isServerGreenlighted == true)
		{
			int epollWaitResult = epoll_wait(this->_serverFD, epollEvents, MAX_EVENTS, EPOLL_MAX_WAIT_TIME_MS);
			if (epollWaitResult == -1)
				throw FailureEpollWaitException();
			for (int eventId = 0; eventId < epollWaitResult; ++eventId)
				this->_triageEpollEvents(epollEvents[eventId]);
		}
	}
	catch(const std::exception& e)
	{
		print(2, e.what());
	}
}

void Server::_triageEpollEvents(epoll_event & epollEvents)
{
	try
	{
		if(epollEvents.events & (EPOLLHUP | EPOLLERR | EPOLLRDHUP))// Stop listening to this fd and remove the associated clients
			this->_disconnectFD(epollEvents.data.fd);
		if (epollEvents.events & EPOLLIN)
		{
			if (!this->_clients.count(epollEvents.data.fd))
				this->_addNewClient(epollEvents.data.fd);
		}
			// Check it the client exists: Add a new client or manage the received request
			// EPOLLOUT?
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << '\n';
	}
}
void Server::_addNewClient(int listenedFD)
{
	struct sockaddr	sockAddr;
	socklen_t addrLen = sizeof(sockAddr);
	int clientFD = accept(fd, &sockAddr, &addrLen);
	if(clientFD == -1)
		throw AcceptFailureException();
	this->_clients[clientFD] = new Client(); // create a client based on the fd and the initial socket
	if(fcntl(clientFD, F_SETFL, O_NONBLOCK) == -1)
		throw Socket::FailureSetNonBlockingSocketException();
	// part below copy/pasted from the server initialisation method -> to be factorised in a function
	epoll_event ev;
	ev.events = EPOLLRDHUP | EPOLLHUP | EPOLLERR | EPOLLIN;
	ev.data.fd = clientFD;
	if (epoll_ctl(this->_serverFD, EPOLL_CTL_ADD, clientFD, &ev) == -1)
		throw FailureAddFDToEpollException();
	displayTimestamp(void);
	std::cout << "[Info] - New client (using FD " << << "added on the FD: "<< listenedFD << std::endl;
}

void Server::_disconnectFD(int listenedFD)
{
	epoll_event ev;
	ev.data.fd = listenedFD;
	if (epoll_ctl(this->_serverFD, EPOLL_CTL_DEL, listenedFD, &ev) == -1)
		throw FailureDeleteFDEpollException();
	if (this->_clients.count(listenedFD))
	{
		delete this->_clients[listenedFD];
		this->_clients.erase(listenedFD);
	}
	throw DisconnectedClientFDException();
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

const char* Server::FailureDeleteFDEpollException::what() const throw()
{
	return ("[Error] - Failure to delete a FD from the epoll listen list");
}

const char* Server::FailureEpollWaitException::what() const throw()
{
	return ("[Error] - Failure to run Epoll_wait");
}

const char* Server::DisconnectedClientFDException::what() const throw()
{
	return ("[Error] - Client FD disconnected, associated clients are erased");
}