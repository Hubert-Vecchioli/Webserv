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

		void parseUser(std::vector<std::string> &args);				//get user
		void parseErrorLog(std::vector<std::string> &args);			//get error_log
		void parseBodySize(std::vector<std::string> &args);			//get body_size
		void parseServerBlock(std::istringstream &iss);								//parse server blocks inside config file

	public:
		ConfigurationFile(void);
		ConfigurationFile(int numberArgs, char** args);
		ConfigurationFile(ConfigurationFile const & rhs);
		ConfigurationFile &operator=(ConfigurationFile const & rhs);
		~ConfigurationFile(void);

		void read(void);					//read file if one was already provided
		void read(std::string filename);	//read file if one was not provided or rewrite file to read
		void parseFile(void);
		
		
		std::string getUser(void) const;	//get user
		std::string getError_log(void) const;	//get error_log
		unsigned long getBody_size(void) const;	//get body_size
		std::vector<ServerBlock> getServerBlocks(void) const;	//get server blocks
		std::vector<ServerBlock*> getServerBlocksPointers(void);	//get server blocks
		std::vector<std::pair<std::string, int> > getserverIPandPorts(void) const; 		//get server IP and Ports from the various server blocks
};

#endif
