/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Socket.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hvecchio <hvecchio@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/27 16:53:56 by hvecchio          #+#    #+#             */
/*   Updated: 2024/10/01 12:32:57 by hvecchio         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SOCKET_HPP
# define SOCKET_HPP

#include "webserv.hpp"
	// 

class Socket
{
	private:
		struct sockaddr_in	_sockaddr

	public:
		Socket(void);
		Socket(Socket const & rhs);
		Socket &operator=(Socket const & rhs);
		~Socket(void);
};

#endif