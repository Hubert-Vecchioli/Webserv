/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hvecchio <hvecchio@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/27 16:54:05 by hvecchio          #+#    #+#             */
/*   Updated: 2024/10/06 16:10:57 by hvecchio         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
		static Client* findInstanceWithFD(std::vector<Client>& vector, int fd);
};

#endif
