
#ifndef SOCKET_HPP
# define SOCKET_HPP

#include "webserv.hpp"

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
		
		int getFD(void);
		static Socket* findInstanceWithFD(std::vector<Socket*>& vector, int fd);


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
