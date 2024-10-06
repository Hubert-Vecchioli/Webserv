/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Socket.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hvecchio <hvecchio@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/27 16:53:56 by hvecchio          #+#    #+#             */
/*   Updated: 2024/10/06 15:29:37 by hvecchio         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SOCKET_HPP
# define SOCKET_HPP

#include "webserv.hpp"

// That can almost be replaced by a struct 
class Socket
{
	private:
		std::string					_ip;
		int							_fd;
		unsigned int				_port;
		struct sockaddr_in			_sockaddr;
		bool						_isClientSocket;

	public:
		Socket(void);
		Socket(int fd, unsigned int port, std::string ip);
		Socket(Socket const & rhs);
		Socket &operator=(Socket const & rhs);
		~Socket(void);
		
		static Socket* findInstanceWithFD(std::vector<Socket>& vector, int fd);


		class FailureSetNonBlockingSocketException : public std::exception
		{
			public:
				virtual const char* what() const throw();
		};
		class FailureUpdateSocketException : public std::exception
		{
			public:
				virtual const char* what() const throw();
		};
		class FailureBindSocketException : public std::exception
		{
			public:
				virtual const char* what() const throw();
		};
		class FailureSocketListenException : public std::exception
		{
			public:
				virtual const char* what() const throw();
		};
};

#endif