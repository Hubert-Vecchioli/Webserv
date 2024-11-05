#ifndef CLIENT_HPP
# define CLIENT_HPP

#include "webserv.hpp"

class Client
{
	private:
		std::time_t _lastActionTimeStamp;
		int			_fd;

	public:
		Client(void);
		Client(int fd);
		Client(Client const & rhs);
		Client &operator=(Client const & rhs);
		~Client(void);
		
		int getFD(void);
		void updateLastActionTimeStamp();
		std::time_t getLastActionTimeStamp(void) const {return _lastActionTimeStamp;};
		static Client* findInstanceWithFD(std::vector<Client*>& vector, int fd);
};

#endif
