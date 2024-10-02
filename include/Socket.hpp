/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Socket.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hvecchio <hvecchio@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/27 16:53:56 by hvecchio          #+#    #+#             */
/*   Updated: 2024/10/02 19:19:56 by hvecchio         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SOCKET_HPP
# define SOCKET_HPP

#include "webserv.hpp"

// That can almost be replaced by a struct 
class Socket
{
	private:
		//Placeholder - check if all are needed - any are mising
		int							_fd;
		std::vector<BlocServer>*	_serverBlocks;
		struct sockaddr_in	_sockaddr

	public:
		Socket(void);
		Socket(int fd, std::vector<BlocServer>* servers);
		Socket(Socket const & rhs);
		Socket &operator=(Socket const & rhs);
		~Socket(void);
		//Todo: Add getters or move the attribute to public?
};

#endif