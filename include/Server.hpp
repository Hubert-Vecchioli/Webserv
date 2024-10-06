/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hvecchio <hvecchio@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/27 16:31:07 by hvecchio          #+#    #+#             */
/*   Updated: 2024/10/05 07:39:27 by hvecchio         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

#include "webserv.hpp"

class Server
{
	private:
		Server &				_uniqueInstance;
		bool 					_isServerGreenlighted;
		int						_serverFD;
		std::map<int, Socket*>	_sockets;
		std::map<int, Client*>	_clients;
		
		void _disconnectClient(int listenedFD);
		void _triageEpollEvents(epoll_event & epollEvents);

	public:
		Server(void);
		Server(Server const & rhs);
		Server &operator=(Server const & rhs);
		~Server(void);
		
		void startServer(ConfigurationFile & configurationFile);
		void runServer(void);
		void stopServer(void);
		static const Server & getInstance(void) {return _uniqueInstance};
		
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
};

#endif