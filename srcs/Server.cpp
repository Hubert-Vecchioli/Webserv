/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hvecchio <hvecchio@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/27 16:31:05 by hvecchio          #+#    #+#             */
/*   Updated: 2024/10/06 09:48:01 by hvecchio         ###   ########.fr       */
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
	//close and delete all the clients & sockets
}

void Server::startServer(ConfigurationFile & configurationFile)
{
	print(1, "[Info] - Initialising the webserv");
	this->_serverFD(epoll_create(MAX_EVENTS));
    if (this->_serverFD == -1)
		throw FailureInitiateEpollInstanceException();
	std::vector<pair <std::string ip, unsigned int port> ip_port> &parsed_config = configurationFile.getIpPorts();
	for (std::vector<pair <std::string ip, unsigned int port> ip_port>::iterator it = parsed_config.begin(); it != parsed_config.end(); ++it)
	{
		int blocServersFD = socket(AF_INET, SOCK_STREAM, 0);
		if (blocServersFD == -1)
			throw FailureInitiateSocketException();
		modifyEpollCTL(this->_serverFD, blocServersFD, EPOLL_CTL_ADD);
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
		if(epollEvents.events & (EPOLLHUP | EPOLLERR | EPOLLRDHUP)) // TODO: ajouter une fonction qui loop sur les clients et verifie si ils ne timeout pas
			this->_disconnectClient(epollEvents.data.fd);// Stop listening to this client
		if (epollEvents.events & EPOLLIN)
		{
			if (this->_clients[epollEvents.data.fd])
				// handle request
			else
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
	struct sockaddr_in	sockAddr; // because it is an IPV4
	socklen_t addrLen = sizeof(sockAddr);
	int clientFD = accept(fd, &sockAddr, &addrLen);
	if(clientFD == -1)
		throw Server::AcceptFailureException();
	this->_clients[clientFD] = new Client(); // create a client based on the fd and the initial socket
	if(fcntl(clientFD, F_SETFL, O_NONBLOCK) == -1) // Set the socket to non-blocking
		throw Socket::FailureSetNonBlockingSocketException();
	modifyEpollCTL(this->_serverFD, clientFD, EPOLL_CTL_ADD);
	displayTimestamp(void);
	std::cout << "[Info] - New client added (connecting to FD " << listenedFD << ", accepted on the FD: "<< clientFD << " )" << std::endl;
}

void Server::_disconnectClient(int listenedFD)
{
	modifyEpollCTL(this->_serverFD, listenedFD, EPOLL_CTL_DEL);
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

const char* Server::FailureModifyFDEpollException::what() const throw()
{
	return ("[Error] - Failure to update the FD from the epoll listen list");
}

const char* Server::FailureEpollWaitException::what() const throw()
{
	return ("[Error] - Failure to run Epoll_wait");
}

const char* Server::DisconnectedClientFDException::what() const throw()
{
	return ("[Error] - Client FD disconnected, associated clients are erased");
}

const char* Server::AcceptFailureException::what() const throw()
{
	return ("[Error] - Failure to accept the new client's FD");
}