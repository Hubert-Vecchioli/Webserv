/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   LocationBlock.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebesnoin <ebesnoin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/29 14:07:40 by hvecchio          #+#    #+#             */
/*   Updated: 2024/10/05 15:37:24 by ebesnoin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#ifndef LOCATIONBLOCK_HPP
# define LOCATIONBLOCK_HPP

#include "webserv.hpp"

class LocationBlock
{
	private:

	public:
		LocationBlock(std::string block);
		LocationBlock(const LocationBlock &copy);
		LocationBlock &operator=(const LocationBlock &copy);
		~LocationBlock(void);
};

#endif
