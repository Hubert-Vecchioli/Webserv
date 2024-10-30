
#ifndef SERVERBLOCK_HPP
# define SERVERBLOCK_HPP

#include "webserv.hpp"

class LocationBlock;

class ServerBlock
{
	private:
		std::pair<std::string, int>	_listen;
		std::vector<std::string>	_server_name;
		std::map<int, std::string> _error_pages;
		std::vector<LocationBlock> _locationBlocks;

		void parseListen(std::vector<std::string> &args);  //get IP and Port -> listen
		void parseServerName(std::vector<std::string> &args); //get server name
		void parseErrorPages(std::vector<std::string> &args); //get error pages
		size_t parseLocationBlock(std::vector<std::string> block, size_t i); //get location blocks in the server block
		void parseServer(std::vector<std::string> block);

	public:
		ServerBlock(std::vector<std::string> block);
		ServerBlock(ServerBlock const & rhs);
		ServerBlock &operator=(ServerBlock const & rhs);
		~ServerBlock(void);

		std::pair<std::string, int> getIPandPort(void) const;
		std::vector<std::string> getServerName(void) const;
		std::map<int, std::string> getErrorPages(void) const;
		std::vector<LocationBlock> getLocationBlocks(void) const;
		std::vector<LocationBlock*> getLocationBlocksPointers(void);
		std::map<std::string, std::vector<std::string> > getLocMethods(void) const;
};

#endif
