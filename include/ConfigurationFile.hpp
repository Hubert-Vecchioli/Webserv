/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigurationFile.hpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hvecchio <hvecchio@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/27 16:55:36 by hvecchio          #+#    #+#             */
/*   Updated: 2024/10/01 13:02:50 by hvecchio         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONFIGURATIONFILE_HPP
# define CONFIGURATIONFILE_HPP

#include "webserv.hpp"

class ConfigurationFile
{
	private:
		// Is it really usefull ?? std::vector<ConfigurationBlock> 				_configList;
		std::string 									_filePathAndName;
		std::vector<ServerBlock> 						_servers;
		std::map<std::string, std::vector<BlocServer> > _configs; // string with the Port and IP? or all within the bloc server?
	public:
		ConfigurationFile(void);
		ConfigurationFile(int numberArgs, char** args);
		ConfigurationFile(ConfigurationFile const & rhs);
		ConfigurationFile &operator=(ConfigurationFile const & rhs);
		~ConfigurationFile(void);
};

#endif
