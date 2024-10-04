/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigurationBlock.hpp                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebesnoin <ebesnoin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/27 16:55:36 by hvecchio          #+#    #+#             */
/*   Updated: 2024/10/04 17:07:01 by ebesnoin         ###   ########.fr       */
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
