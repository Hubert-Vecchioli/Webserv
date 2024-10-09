/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigurationFile.hpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebesnoin <ebesnoin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/27 16:55:36 by hvecchio          #+#    #+#             */
/*   Updated: 2024/10/09 01:26:22 by ebesnoin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONFIGURATIONFILE_HPP
# define CONFIGURATIONFILE_HPP

#include "webserv.hpp"

class ServerBlock;

class ConfigurationFile
{
	private:
		std::string 									_filename;
		std::string 									_content;
		std::string 									_user;
		std::string										_error_log;
		unsigned long 									_body_size;
		std::vector<ServerBlock> 						_serverBlocks;

		void parseUser(void);				//get user
		void parseErrorLog(void);			//get error_log
		void parseBodySize(void);			//get body_size

	public:
		ConfigurationFile(void);
		ConfigurationFile(int numberArgs, char** args);
		ConfigurationFile(ConfigurationFile const & rhs);
		ConfigurationFile &operator=(ConfigurationFile const & rhs);
		~ConfigurationFile(void);

		void read(void);					//read file if one was already provided
		void read(std::string filename);	//read file if one was not provided or rewrite file to read
		void parseServerBlock(void);		//parse server blocks inside config file
		void parseBase(void);				//parse user, worker_processes and error_log
		
		std::string getUser(void) const;	//get user
		std::string getError_log(void) const;	//get error_log
		unsigned long getBody_size(void) const;	//get body_size
		std::vector<ServerBlock> getServerBlocks(void) const;	//get server blocks
		std::vector<std::pair<std::string, int> > getserverIPandPorts(void) const; 		//get server IP and Ports from the various server blocks
};

#endif
