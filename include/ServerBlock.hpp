/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerBlock.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebesnoin <ebesnoin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/29 14:07:40 by hvecchio          #+#    #+#             */
/*   Updated: 2024/10/04 17:27:52 by ebesnoin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#ifndef SERVERBLOCK_HPP
# define SERVERBLOCK_HPP

#include "webserv.hpp"

class ServerBlock
{
	private:
		std::string 	_listen;
		std::string 	_server_name;
		std::string 	_root;

	public:
		ServerBlock(void);
		ServerBlock(ServerBlock const & rhs);
		ServerBlock &operator=(ServerBlock const & rhs);
		~ServerBlock(void);
};

#endif
