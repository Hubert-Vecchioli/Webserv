/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigurationFile.hpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hvecchio <hvecchio@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/27 16:55:36 by hvecchio          #+#    #+#             */
/*   Updated: 2024/09/27 17:15:50 by hvecchio         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONFIGURATIONFILE_HPP
# define CONFIGURATIONFILE_HPP

# include <iostream>
# include <map>

class ConfigurationFile
{
private:

public:
	ConfigurationFile(void);
	ConfigurationFile(ConfigurationFile const & rhs);
	ConfigurationFile &operator=(ConfigurationFile const & rhs);
	~ConfigurationFile(void);
	
};

#endif
