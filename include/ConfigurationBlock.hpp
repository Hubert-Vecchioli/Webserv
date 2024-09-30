/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigurationBlock.hpp                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hvecchio <hvecchio@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/27 16:55:36 by hvecchio          #+#    #+#             */
/*   Updated: 2024/09/29 21:26:49 by hvecchio         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONFIGURATIONBLOCK_HPP
# define CONFIGURATIONBLOCK_HPP

#include "webserv.hpp"

class ConfigurationBlock
{
	private:

	public:
		ConfigurationBlock(void);
		ConfigurationBlock(int numberArgs, char** args);
		ConfigurationBlock(ConfigurationBlock const & rhs);
		ConfigurationBlock &operator=(ConfigurationBlock const & rhs);
		~ConfigurationBlock(void);
};

#endif
