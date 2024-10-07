/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hvecchio <hvecchio@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/27 16:31:05 by hvecchio          #+#    #+#             */
/*   Updated: 2024/10/07 12:01:44 by hvecchio         ###   ########.fr       */
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
	//close and delete all the clients & sockets & requests
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
		this->_sockets.push_back(new Socket(blocServersFD, parsed_config.second, parsed_config.first));
	}
	this->_isServerGreenlighted = true;
	print(1, "[Info] - Webserv initialised");
}

void Server::stop(void)
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
			this->_reviewClientsHaveNoTimeout();
		}
	}
	catch(const std::exception& e)
	{
		print(2, e.what());
	}
}

//the function below has an ugly use of exceptions, stucture to be reviewed
void Server::_reviewClientsHaveNoTimeout(void)
{
	for(std::vector<Client*>::iterator it = this->_clients.begin(); it != this->_clients.begin(); ++it)
	{
		if (std::time(nullptr) - it->_lastActionTimeStamp > TIMEOUT_LIMIT_SEC)
		{
			try
			{
				this->_disconnectClient(it->getFD());
			}
			catch(const std::exception& e)
			{
				print(2, e.what());
			}
			
		}	
	}
}

void Server::_triageEpollEvents(epoll_event & epollEvents)
{
	try
	{
		if(epollEvents.events & (EPOLLHUP | EPOLLERR | EPOLLRDHUP)) 
			this->_disconnectClient(epollEvents.data.fd);// Stop listening to this client
		if (epollEvents.events & EPOLLIN)
		{
			if (Client::findInstanceWithFD(this->_clients, epollEvents.data.fd))
				this->_receiveRequest(epollEvents.data.fd);
			else
				this->_addNewClient(epollEvents.data.fd);
		}
		// Review if we need to send data
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << '\n';
	}
}

void Server::_receiveRequest(int fd)
{
	Client* clientSendingARequest = Client::findInstanceWithFD(this->_clients, fd);
	clientSendingARequest->updateLastActionTimeStamp();
	displayTimestamp(void);
	std::cout << "[Info] - Receiving request from Client FD : " << fd << " )" << std::endl;
	char rawHTTPRequest[MAX_REQUEST_SIZE + 1];
	int sizeHTTPRequest = recv(fd, rawHTTPRequest, MAX_REQUEST_SIZE, 0);
	if(sizeHTTPRequest == 0)
		this->_disconnectClient(fd);
	if(sizeHTTPRequest < 0)
		throw FailureToReceiveData();
	rawHTTPRequest[sizeHTTPRequest] = 0;
	this->_requests.push_back(new HttpRequest(rawHTTPRequest, sizeHTTPRequest, clientSendingARequest));
}

void Server::_addNewClient(int listenedFD)
{
	struct sockaddr_in	sockAddr; // because it is an IPV4
	socklen_t addrLen = sizeof(sockAddr);
	int clientFD = accept(listenedFD, &sockAddr, &addrLen);
	if(clientFD == -1)
		throw Server::AcceptFailureException();
	this->_clients.push_back(new Client(clientFD));
	if(fcntl(clientFD, F_SETFL, O_NONBLOCK) == -1)
		throw Socket::FailureSetNonBlockingSocketException();
	modifyEpollCTL(this->_serverFD, clientFD, EPOLL_CTL_ADD);
	displayTimestamp(void);
	std::cout << "[Info] - New client added (connecting to FD " << listenedFD << ", accepted on the FD: "<< clientFD << " )" << std::endl;
}

void Server::_disconnectClient(int listenedFD)
{
	Client* clientToDisconnect = Client::findInstanceWithFD(this->_clients, listenedFD);
	modifyEpollCTL(this->_serverFD, listenedFD, EPOLL_CTL_DEL);
	if (clientToDisconnect)
	{
		delete *clientToDisconnect;
		this->_clients.erase(clientToDisconnect);
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

const char* Server::FailureToReceiveData::what() const throw()
{
	return ("[Error] - The function recv failed"); //TBD if the client FD is required to debug
}