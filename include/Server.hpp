/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hvecchio <hvecchio@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/27 16:31:07 by hvecchio          #+#    #+#             */
/*   Updated: 2024/10/23 13:32:02 by hvecchio         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

#include "webserv.hpp"

class Socket;
class Client;
class ConfigurationFile;
class BlocServer;
class HttpRequest;

class Server
{
	private:
		static Server				*_uniqueInstance;
		ConfigurationFile*			_configurationFile;
		bool 						_isServerGreenlighted;
		int							_serverFD;
		std::vector<Socket*>		_sockets;
		std::vector<Client*>		_clients;
		std::vector<HttpRequest*>	_requests;

		Server(void) : _configurationFile(0), _isServerGreenlighted(false), _serverFD(-1) {}
		Server(Server const & rhs);
		Server &operator=(Server const & rhs);

		void _triageEpollEvents(epoll_event & epollEvents);
		void _reviewClientsHaveNoTimeout(void);
		void _receiveRequest(int fd);
		void _sendRequest(int fd);
		void _reviewRequestsCompleted(void);
		void _addNewClient(int listenedFD);
		void _disconnectClient(int listenedFD);

	public:
		static Server &getInstance() {
			if (!_uniqueInstance)
				_uniqueInstance = new Server();
			return *_uniqueInstance;
		}
		~Server(void);
		
		void startServer(ConfigurationFile * configurationFile);
		void runServer(void);
		static void stopServer(void);
		//ConfigurationFile &getConfigurationFile() {return _configurationFile;};

		ConfigurationFile &getConfigurationFile(void) {return *_configurationFile;};
		
		class FailureInitiateEpollInstanceException : public std::exception
		{
			public:
				virtual const char* what() const throw();
		};
		class FailureInitiateSocketException : public std::exception
		{
			public:
				virtual const char* what() const throw();
		};
		class FailureEpollWaitException : public std::exception
		{
			public:
				virtual const char* what() const throw();
		};
		class DisconnectedClientFDException : public std::exception
		{
			public:
				virtual const char* what() const throw();
		};
		class FailureModifyFDEpollException : public std::exception
		{
			public:
				virtual const char* what() const throw();
		};
		class AcceptFailureException : public std::exception
		{
			public:
				virtual const char* what() const throw();
		};
		class FailureToReceiveData : public std::exception
		{
			public:
				virtual const char* what() const throw();
		};
		class FailureToSendData : public std::exception
		{
			public:
				virtual const char* what() const throw();
		};
};

#endif
