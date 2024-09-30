/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   LocationBlock.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hvecchio <hvecchio@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/29 14:07:40 by hvecchio          #+#    #+#             */
/*   Updated: 2024/09/29 21:27:19 by hvecchio         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#ifndef LOCATIONBLOCK_HPP
# define LOCATIONBLOCK_HPP

#include "webserv.hpp"

class LocationBlock
{
	private:

	public:
		LocationBlock(void);
		LocationBlock(LocationBlock const & rhs);
		LocationBlock &operator=(LocationBlock const & rhs);
		~LocationBlock(void);
};

#endif