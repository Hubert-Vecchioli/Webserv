/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigurationFile.hpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebesnoin <ebesnoin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/27 16:55:36 by hvecchio          #+#    #+#             */
/*   Updated: 2024/10/04 17:26:27 by ebesnoin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONFIGURATIONFILE_HPP
# define CONFIGURATIONFILE_HPP

#include "webserv.hpp"

class ConfigurationFile
{
	private:
		std::string 									_filename;
		std::string 									_content;
		std::string 									_user;
		int 											_workers;
		std::string										_error_log;
		std::vector<ServerBlock> 						_serverBlocks;
		//std::vector<pair <int, int> > 	_ip_port;
		//std::vector<BlocServer>							_configurationMapIpPort; // string with the Port and IP? or all within the bloc server?
	public:
		ConfigurationFile(void);
		ConfigurationFile(int numberArgs, char** args);
		ConfigurationFile(ConfigurationFile const & rhs);
		ConfigurationFile &operator=(ConfigurationFile const & rhs);
		~ConfigurationFile(void);

		void read(void);
		void read(std::string filename);
		void parseServerBlock(void);
		
		// method getBlockServers to get _configurationMapIpPort
};

#endif
