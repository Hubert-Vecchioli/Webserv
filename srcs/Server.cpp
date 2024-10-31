#include "webserv.hpp"

Server* Server::_uniqueInstance = 0;

void Server::cleanup(void)
{
	if (this->_serverFD != -1)
		close(_serverFD);
	for (std::vector<Socket*>::iterator it = this->_sockets.begin(); it != this->_sockets.end(); ++it)
	{
		close((*it)->getFD());
		delete (*it);
	}
	this->_sockets.clear();
	for(std::vector<HttpRequest*>::iterator it = this->_requests.begin(); it != this->_requests.end(); ++it)
	{
		delete (*it)->getResponse();
		delete *it;
	}
	this->_requests.clear();
	for (std::vector<Client*>::iterator it = this->_clients.begin(); it != this->_clients.end(); ++it)
	{
		close((*it)->getFD());
		delete (*it);
	}
	this->_clients.clear();
}

Server::~Server()
{
}

void Server::startServer(ConfigurationFile * configurationFile)
{
	this->_configurationFile = configurationFile;
	print(1, "[Info] - Initialising the webserv");
	this->_serverFD = epoll_create(MAX_EVENTS);
    if (this->_serverFD == -1)
		throw FailureInitiateEpollInstanceException();
	const std::vector<std::pair <std::string, int> > &parsed_config = this->_configurationFile->getserverIPandPorts();
	for (std::vector<std::pair <std::string, int> >::const_iterator it = parsed_config.begin(); it != parsed_config.end(); ++it)
	{
		int blocServersFD = socket(AF_INET, SOCK_STREAM, 0);
		if (blocServersFD == -1)
			throw FailureInitiateSocketException();
		modifyEpollCTL(this->_serverFD, blocServersFD, EPOLL_CTL_ADD);
		this->_sockets.push_back(new Socket(blocServersFD, it->second, it->first));
	}
	this->_isServerGreenlighted = true;
	print(1, "[Info] - Webserv initialised");
}

void Server::stopServer(void)
{
	Server::getInstance()._isServerGreenlighted = false;
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
			{
				if (this->_isServerGreenlighted == false)
					break;
				throw FailureEpollWaitException();
			}
			for (int eventId = 0; eventId < epollWaitResult; ++eventId)
			{
					this->_triageEpollEvents(epollEvents[eventId]);
			}
			this->_reviewClientsHaveNoTimeout();
			this->_reviewRequestsCompleted();
		}
	}
	catch(const std::exception& e)
	{
		print(2, e.what());
	}
}

void Server::_reviewRequestsCompleted(void)
{
	for(std::vector<HttpRequest*>::iterator it = this->_requests.begin(); it != this->_requests.end(); ++it)
	{
		if ((*it)->getResponse()->getResponseStatus() || std::time(0) - (*it)->getResponse()->getLastActionTimeStamp() > REQUEST_TIMEOUT_LIMIT_SEC)
		{
			try
			{
				delete (*it)->getResponse();
				delete *it;
				this->_requests.erase(it);
			}
			catch(const std::exception& e)
			{
				print(2, e.what());
			}
			break;
		}	
	}
}

void Server::_reviewClientsHaveNoTimeout(void)
{
	std::vector<Client*> clientsCopy = this->_clients;
	for(std::vector<Client*>::iterator it = clientsCopy.begin(); it != clientsCopy.end(); ++it)
	{
		if (std::time(0) - (*it)->getLastActionTimeStamp() >= CLIENT_TIMEOUT_LIMIT_SEC)
		{
			try
			{
				this->_disconnectClient((*it)->getFD());
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
		if((epollEvents.events & (EPOLLHUP | EPOLLERR | EPOLLRDHUP)) && this->_getNumberClientsConnected() <= MAX_CLIENT_NUMBER)
			this->_disconnectClient(epollEvents.data.fd);
		if (epollEvents.events & EPOLLIN)
		{
			if (Client::findInstanceWithFD(this->_clients, epollEvents.data.fd))
				this->_receiveRequest(epollEvents.data.fd);
			else
				this->_addNewClient(epollEvents.data.fd);
		}
		if (epollEvents.events & EPOLLOUT)
		{
			if(HttpRequest::findInstanceWithFD(this->_requests, epollEvents.data.fd) && !HttpRequest::findInstanceWithFD(this->_requests, epollEvents.data.fd)->getResponse()->getResponseStatus())
			{
				this->_sendRequest(epollEvents.data.fd);
			}	
		}
	}
	catch(const std::bad_alloc& e)
	{
		Server::stopServer();
		print(2, e.what());
	}
	catch(const std::exception& e)
	{
		print(2, e.what());
	}

}

void Server::_sendRequest(int fd)
{
	Client::findInstanceWithFD(this->_clients, fd)->updateLastActionTimeStamp();
	print(1, "[Info] - Sending response to Client FD : ", fd);
	HttpResponse *response = HttpRequest::findInstanceWithFD(this->_requests, fd)->getResponse();
	int sizeHTTPResponseSent = send(fd, response->getResponseContent().c_str(), response->getResponseContent().size(), 0);// For info, send is equivalent to write as I am not using any flag
	if(sizeHTTPResponseSent == 0 && response->getResponseContent().size() > 0)
		this->_disconnectClient(fd);
	if(sizeHTTPResponseSent < 0)
		throw FailureToSendData();
	//std::cout<< "request: "<< response->getResponseContent().c_str()<<std::endl;
	modifyEpollCTL(this->_serverFD, fd, EPOLL_CTL_MOD, false);
	print(1, "[Info] - Response successfully sent to Client FD : ", fd);
	response->setResponseStatustoTrue();
	if (HttpRequest::findInstanceWithFD(this->_requests, fd)->getRequestURI() == "413")
		this->_disconnectClient(fd);
}

void Server::_receiveRequest(int fd)
{
    Client* clientSendingARequest = Client::findInstanceWithFD(this->_clients, fd);
    clientSendingARequest->updateLastActionTimeStamp();
    print(1, "[Info] - Receiving request from Client FD : ", fd);

    unsigned char* rawHTTPRequest = new unsigned char[MAX_REQUEST_SIZE * 3 + 1]; 
    int sizeHTTPRequest = recv(fd, rawHTTPRequest, MAX_REQUEST_SIZE, 0);
    if (sizeHTTPRequest == 0)
	{
        this->_disconnectClient(fd);
        delete[] rawHTTPRequest;
        return;
    }
    if (sizeHTTPRequest < 0)
    {
        delete[] rawHTTPRequest;
        throw FailureToReceiveData();
    }
    
    rawHTTPRequest[sizeHTTPRequest] = 0;
    std::string header(reinterpret_cast<char*>(rawHTTPRequest));
    std::size_t contentTypePos = header.find("Content-Type: application/octet-stream");
    std::size_t contentLengthPos = header.find("Content-Length: ");
    int contentLength = 0;
    bool isOctetStream = (contentTypePos != std::string::npos);

    if (contentLengthPos != std::string::npos) {
        contentLength = atoi(header.substr(contentLengthPos + 16).c_str());
		if(contentLength > (int)this->getConfigurationFile().getBody_size())
		{
			delete[] rawHTTPRequest;
			unsigned char error[4];
			error[0] = '4';
			error[1] = '1';
			error[2] = '3';
			error[3] = 0;
			HttpRequest* request = new HttpRequest(clientSendingARequest, error, 3);
			HttpResponse* response = new HttpResponse(*this, *request);
			request->setResponse(response);
			this->_requests.push_back(request);
			print(1, "[Info] - Excessive file size");
			modifyEpollCTL(this->_serverFD, fd, EPOLL_CTL_MOD, true);
        	return;
		}
    }
    if (isOctetStream && contentLength > 0)
    {
        unsigned char* tempBuffer = NULL;
        while (sizeHTTPRequest < contentLength)
        {
            int bytesReceived = recv(fd, rawHTTPRequest + sizeHTTPRequest, MAX_REQUEST_SIZE, 0);
            if (bytesReceived <= 0)
            {
                this->_disconnectClient(fd);
                delete[] rawHTTPRequest;
                return;
            }
            sizeHTTPRequest += bytesReceived;
            tempBuffer = new unsigned char[sizeHTTPRequest + 3*MAX_REQUEST_SIZE + 1]; // Redimensionner
			std::copy(rawHTTPRequest, rawHTTPRequest + sizeHTTPRequest, tempBuffer);
			delete[] rawHTTPRequest; 
			rawHTTPRequest = tempBuffer;
        }
        rawHTTPRequest[sizeHTTPRequest] = 0;
    }
	// std::cout<< rawHTTPRequest<<std::endl;
    HttpRequest* request = new HttpRequest(clientSendingARequest, rawHTTPRequest, sizeHTTPRequest);
    HttpResponse* response = new HttpResponse(*this, *request);
    request->setResponse(response);
    this->_requests.push_back(request);
    
    print(1, "[Info] - Request successfully received from Client FD : ", fd);
    modifyEpollCTL(this->_serverFD, fd, EPOLL_CTL_MOD, true);
    
    delete[] rawHTTPRequest;
}

void Server::_addNewClient(int listenedFD)
{
	struct sockaddr	sockAddr;
	socklen_t addrLen = sizeof(sockAddr);
	int clientFD = accept(listenedFD, &sockAddr, &addrLen);
	if(clientFD == -1)
		throw Server::AcceptFailureException();
	this->_clients.push_back(new Client(clientFD));
	if(fcntl(clientFD, F_SETFL, O_NONBLOCK) == -1)
		throw Socket::FailureSetNonBlockingSocketException();
	modifyEpollCTL(this->_serverFD, clientFD, EPOLL_CTL_ADD);
	displayTimestamp();
	std::cout << "[Info] - New client added (connecting to FD " << listenedFD << ", accepted on the FD: "<< clientFD << " )" << std::endl;
	this->_addClientToServer();
}

void Server::_disconnectClient(int listenedFD)
{
	Client* clientToDisconnect = Client::findInstanceWithFD(this->_clients, listenedFD);
	modifyEpollCTL(this->_serverFD, listenedFD, EPOLL_CTL_DEL);
	close(listenedFD);
	if (clientToDisconnect)
	{
        for (std::vector<Client*>::iterator it = this->_clients.begin(); it != this->_clients.end(); ++it)
        {
            if (*it == clientToDisconnect)
            {
                this->_deleteAssociatedRequests((*it)->getFD());
				this->_deleteAssociatedRequests((*it)->getFD());
				close((*it)->getFD());
				delete (*it);
				this->_clients.erase(it);
                break;
            }
        }
	}
	this->_removeClientToServer();
	//std::cout<<"[Error] - Client FD disconnected, associated client was erased fd: "<<listenedFD<< std::endl;
	throw DisconnectedClientFDException();
}

void Server::_deleteAssociatedRequests(int fdToDelete)
{
	for(std::vector<HttpRequest*>::iterator it = this->_requests.begin(); it != this->_requests.end(); ++it)
	{
		if ((*it)->getClient()->getFD() == fdToDelete)
		{
			try
			{
				delete (*it)->getResponse();
				delete *it;
				this->_requests.erase(it);
			}
			catch(const std::exception& e)
			{
				print(2, e.what());
			}
			break;
		}	
	}
}

int Server::_getNumberClientsConnected() const
{
	return _numberClientsConnected;
}
void Server::_addClientToServer() 
{ 
	this->_numberClientsConnected += 1;
}

void Server::_removeClientToServer() 
{ 
	this->_numberClientsConnected -= 1;
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
	return ("[Info] - Client FD disconnected, associated client was erased");
}

const char* Server::AcceptFailureException::what() const throw()
{
	return ("[Error] - Failure to accept the new client's FD");
}

const char* Server::FailureToReceiveData::what() const throw()
{
	return ("[Error] - The function recv() failed");
}

const char* Server::FailureToSendData::what() const throw()
{
	return ("[Error] - The function send() failed");
}
