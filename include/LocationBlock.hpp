
#ifndef LOCATIONBLOCK_HPP
# define LOCATIONBLOCK_HPP

#include "webserv.hpp"

class LocationBlock
{
	private:
		std::string					_location;
		std::string					_root;
		std::vector<std::string>	_index;
		bool						_dirlisting;
		std::vector<std::string>	_methods;
		std::pair<int, std::string>	_redirect;
		std::map<std::string, std::string> cgi_extension;
		std::string					_upload_path;

		void parseLocationBlock(std::vector<std::string> block);
		void parseLocation(std::vector<std::string> args);
		void parseRoot(std::vector<std::string> &args);
		void parseIndex(std::vector<std::string> &args);
		void parseDirlisting(std::vector<std::string> &args);
		void parseMethods(std::vector<std::string> &args);
		void parseRedirect(std::vector<std::string> &args);
		void parseCgiExtension(std::vector<std::string> &args);
		void parseUploadPath(std::vector<std::string> &args);
		
	public:
		LocationBlock(std::vector<std::string> block);
		LocationBlock(const LocationBlock &copy);
		LocationBlock &operator=(const LocationBlock &copy);
		~LocationBlock(void);

		std::string getLocation(void) const;
		std::string getRoot(void) const;
		std::vector<std::string> getIndex(void) const;
		bool getDirlisting(void) const;
		std::vector<std::string> getMethods(void) const;
		std::pair<int, std::string> getRedirect(void) const;
		std::map<std::string, std::string> getCgiExtension(void) const;
		std::string getUploadPath(void) const;

};

#endif
